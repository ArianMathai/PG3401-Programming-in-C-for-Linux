#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "include/sha1.h"

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256
#define MAX_FILE_PATH 256

typedef struct _GLOBALVARS {
    int count[BYTE_RANGE];
    unsigned char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex;
    sem_t sem_full;
    sem_t sem_empty;
    int bytes_in_buffer;
    void* memory_buffer;
    char pszFileName[MAX_FILE_PATH];
    int iExit;
} GLOBALVARS;


void* thread_A(void* arg) {
    // get a shared struct as param from main thread
    GLOBALVARS *psGlobVar = (GLOBALVARS*) arg;

    FILE* fp = fopen(psGlobVar->pszFileName, "rb");
    if (!fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // wait for thread B to finish. sem_empty is initially 1, so does not need to wait on first iteration
        sem_wait(&psGlobVar->sem_empty);
        pthread_mutex_lock(&psGlobVar->mutex); // Locking mutex


        int read_bytes = 0;
        // Read up to 4096 bytes from file. This keeps going until we read 0 bytes meaning file is fully read.
        read_bytes = fread(psGlobVar->buffer + psGlobVar->bytes_in_buffer, 1, BUFFER_SIZE - psGlobVar->bytes_in_buffer, fp);
        // Set bytes_in_buffer to bytes read
        psGlobVar->bytes_in_buffer += read_bytes;


        if (read_bytes == 0){ // If we did not read any bytes we ar done and can exit
            psGlobVar->iExit = 1;
            pthread_mutex_unlock(&psGlobVar->mutex);
            sem_post(&psGlobVar->sem_full); // Post to thread B one last time
            break;
        }

        pthread_mutex_unlock(&psGlobVar->mutex);
        sem_post(&psGlobVar->sem_full); // Post to thread B every iteration after we have read into buffer

    }
    fclose(fp); // close file
    pthread_exit(NULL);
}

void* thread_B(void* arg) {
    // get a shared struct as param from main thread
    GLOBALVARS *psGlobVar = (GLOBALVARS*) arg;

    memset(psGlobVar->count, 0, sizeof(psGlobVar->count));
    unsigned char digest[20]; // This will contain our final sha1 hash
    SHA1_CTX *psSha1Ctx = NULL;
    psSha1Ctx = malloc(sizeof (SHA1_CTX)); // malloc sha1 struct
    if (psSha1Ctx == NULL){
        perror("Failed to malloc sha context");
        return NULL;
    } else {
        SHA1Init(psSha1Ctx); // init sha1
    }
    while (1) {
        sem_wait(&psGlobVar->sem_full); // wait for thread A to read from file and post
        pthread_mutex_lock(&psGlobVar->mutex);

        // if threadA sets exit flag to 1 and buffer is empty we exit
        if (psGlobVar->bytes_in_buffer == 0 || psGlobVar->iExit == 1){
            printf("exiting\n");
            break;
        }

        // Uncomment the code below to count instances of a character in the file
        /*
        for (int i = 0; i < psGlobVar->bytes_in_buffer; i++){
            psGlobVar->count[psGlobVar->buffer[i]]++;
        }
        */

        // Run the current buffer through sha1
        SHA1Update(psSha1Ctx, psGlobVar->buffer, psGlobVar->bytes_in_buffer);


        psGlobVar->bytes_in_buffer = 0; // Reset variable to be ready for next loop in thread A

        pthread_mutex_unlock(&psGlobVar->mutex);
        sem_post(&psGlobVar->sem_empty); // Signal to thread A that we are done and ready for more data:)
    }
    // After we are done processing the whole file we make a final sha1 hash and print it to the screen in hex format
    SHA1Final(digest, psSha1Ctx);
    for (int i = 0; i < sizeof (digest); ++i) {
        printf("%02x", digest[i]);
    }
    printf("\n");
    free(psSha1Ctx);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int iFileLength = 0;
    if (argc != 2){
        perror("Run with filename as arg!");
        return 1;
    } else {
        iFileLength = strlen(argv[1]); // Get length of file param
        if (iFileLength > MAX_FILE_PATH){ // make sure it file name is not too long
            perror("File path to long");
            return 1;
        }

    }

    GLOBALVARS *psGlobalVars = NULL;
    psGlobalVars = malloc(sizeof (GLOBALVARS));
    if (psGlobalVars == NULL){
        perror("Failed on malloc of global vars");
        return 1;
    }
    psGlobalVars->bytes_in_buffer = 0;
    psGlobalVars->iExit = 0;
    strncpy(psGlobalVars->pszFileName, argv[1], MAX_FILE_PATH - 1);
    psGlobalVars->pszFileName[MAX_FILE_PATH - 1] = '\0';

    // init mutex and semaphores
    pthread_mutex_init(&psGlobalVars->mutex, NULL);
    sem_init(&psGlobalVars->sem_full, 0, 0);
    sem_init(&psGlobalVars->sem_empty, 0, 1);

    pthread_t threadA, threadB;

    // start thread A
    if (pthread_create(&threadA, NULL, thread_A, (void*)psGlobalVars) != 0) {
        perror("Could not create thread A");
        free(psGlobalVars);
        pthread_mutex_destroy(&psGlobalVars->mutex);
        sem_destroy(&psGlobalVars->sem_full);
        sem_destroy(&psGlobalVars->sem_empty);
        exit(1);
    }
    // start thread B
    if (pthread_create(&threadB, NULL, thread_B, (void*)psGlobalVars) != 0) {
        perror("Could not create thread B");
        free(psGlobalVars);
        pthread_mutex_destroy(&psGlobalVars->mutex);
        sem_destroy(&psGlobalVars->sem_full);
        sem_destroy(&psGlobalVars->sem_empty);
        exit(1);
    }

    if (pthread_join(threadA, NULL) != 0) {
        perror("Could not join thread A");
        free(psGlobalVars);pthread_mutex_destroy(&psGlobalVars->mutex);
        sem_destroy(&psGlobalVars->sem_full);
        sem_destroy(&psGlobalVars->sem_empty);
        exit(1);
    }
    if (pthread_join(threadB, NULL) != 0) {
        perror("Could not join thread B");pthread_mutex_destroy(&psGlobalVars->mutex);
        sem_destroy(&psGlobalVars->sem_full);
        sem_destroy(&psGlobalVars->sem_empty);
        free(psGlobalVars);
        exit(1);
    }
    // Clean up and destroy mutex and semaphores
    pthread_mutex_destroy(&psGlobalVars->mutex);
    sem_destroy(&psGlobalVars->sem_full);
    sem_destroy(&psGlobalVars->sem_empty);

    // free our shared variables
    free(psGlobalVars);

    return 0;
}











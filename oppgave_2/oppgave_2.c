#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/eksamen_v24_oppgave2_anagram.h"
#include "include/eksamen_v24_oppgave2_all_upper.h"
#include "include/eksamen_v24_oppgave2_all_lower.h"
#include "include/eksamen_v24_oppgave2_palindrom.h"
#include "include/eksamen_v24_oppgave2_heterogram.h"
#include "include/eksamen_v24_oppgave2_disjoint.h"
#include "include/oppgave_2.h"

// Run program with ./oppgave2

#define MAX_WORD_LENGTH 100

// I had an issue when using strncpy, so i made my own copy function
int zero_terminated_strncpy(char *dest, const char *src, int iSize) {
    if (iSize < 1){
        perror("iSize is less than 1 when copying string\n");
        return 1;
    }
    iSize++;
    int i;
    for (i = 0; i < iSize - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}
// Just to verify the contents of binary file
void readBinary(void){
    FILE *pF;
    unsigned char buffer[1028];
    size_t bytesRead;

    pF = fopen("./output.dat", "rb");
    rewind(pF);
    if (pF != NULL)
    {
        while ((bytesRead = fread(buffer, sizeof(unsigned char), sizeof(buffer), pF)) > 0)
        {
            for (size_t i = 0; i < bytesRead; i++)
            {
                printf("%c ", buffer[i]);
            }
            printf("\n");
        }
        fclose(pF);
    }
}


int main(int iArgc, char* argv[]){

    if (iArgc != 1){
        printf("Run program without args\n");
        return 1;
    }
    TASK2_WORD_METADATA *psWord = 0;

    FILE *pFile = fopen("./eksamen_v24_oppgave2.txt", "r");
    if (pFile == NULL) {
        perror("Failed to open file\n");
        return 1;
    }
    FILE *pTmpFile = fopen("./eksamen_v24_oppgave2.txt", "r");
    if (pTmpFile == NULL) {
        perror("Failed to open file\n");
        fclose(pFile);
        return 1;
    }
    FILE *pfOutput = fopen("./output.dat", "wb");
    if (pfOutput == NULL){
        perror("Error opening output file\n");
        fclose(pTmpFile);
        fclose(pFile);
    }

    int iWordCount = 0;
    int iWordSize = 0;
    char wordBuffer[MAX_WORD_LENGTH];
    char buf[MAX_WORD_LENGTH];


    while (1){
        memset(wordBuffer, 0, sizeof (wordBuffer));
        iWordSize = 0;

        if (fgets(wordBuffer, sizeof(wordBuffer), pFile) != NULL) {
            for (int i = 0; i < strlen(wordBuffer); i++) {
                if (wordBuffer[i] == '\r' || wordBuffer[i] == '\n') {
                    wordBuffer[i] = '\0';
                    break;
                }
            }
            iWordSize = strlen(wordBuffer);
            iWordCount++;
            // using calloc here to keep valgrind happy
            psWord = calloc(1, sizeof (TASK2_WORD_METADATA) + iWordSize + 1);
            if (psWord == NULL){
                perror("Failed when allocation memory for psWord\n");
                fclose(pFile);
                fclose(pTmpFile);
                fclose(pfOutput);
                return 1;
            }
            psWord->iIndex = iWordCount + 1;
            psWord->iSize = iWordSize;
            psWord->bIsHeterogram = isHeterogram(wordBuffer);
            psWord->bIsLowercase = isLowerCase(wordBuffer);
            psWord->bIsUppercase = isUpperCase(wordBuffer);
            psWord->bIsPalindrom = isPalindrome(wordBuffer);
            if (zero_terminated_strncpy(psWord->szWord, wordBuffer, iWordSize) == 1){
                free(psWord);
                fclose(pTmpFile);
                fclose(pFile);
                fclose(pfOutput);
                return 1;
            }

            int iTmpBytesRead = 0;
            rewind(pTmpFile);

            while (1){
                memset(buf, 0, sizeof (buf));
                long int iTmpStartPosition = ftell(pTmpFile);
                if (iTmpStartPosition == -1){
                    perror("Failed to get current file position\n");
                    fclose(pFile);
                    fclose(pTmpFile);
                    fclose(pfOutput);
                    free(psWord);
                    return 1;
                }
                if (fgets(buf, sizeof (buf), pTmpFile) != NULL){
                    for (int i = 0; i < strlen(buf); i++) {
                        if (buf[i] == '\r' || buf[i] == '\n') {
                            buf[i] = '\0';
                            break;
                        }
                    }
                    if (strncmp(psWord->szWord, buf, psWord->iSize) == 0){
                        continue;
                    }
                    if (isAnagram(psWord->szWord, buf) == 1){
                        psWord->bIsAnagram = 1;
                    }
                    if (areDisjoint(psWord->szWord, buf) == 1){
                        psWord->bIsDisjoint = 1;
                    }
                    long int iTmpEnd = ftell(pTmpFile); // Get the end position
                    if (iTmpEnd == -1) {
                        perror("Failed to get current file position after reading\n");
                        fclose(pFile);
                        fclose(pTmpFile);
                        fclose(pfOutput);
                        free(psWord);
                        return 1;
                    }
                    int iTmpBytesReadThisTime = iTmpEnd - iTmpStartPosition;
                    iTmpBytesRead += iTmpBytesReadThisTime;
                    fseek(pTmpFile, iTmpBytesRead, SEEK_SET);
                }
                else{
                    break;
                }
            }
            printf("WORD = %s\n", psWord->szWord);
            printf("iSize = %d\n", psWord->iSize);
            printf("isHeterogram = %d\n", psWord->bIsHeterogram);
            printf("isPalindrom = %d\n", psWord->bIsPalindrom);
            printf("isUpper = %d\n", psWord->bIsUppercase);
            printf("isLower = %d\n", psWord->bIsLowercase);
            printf("isAnagram = %d\n", psWord->bIsAnagram);
            printf("isDisjointed = %d\n", psWord->bIsDisjoint);
            printf("\n");

            if (fwrite(psWord, sizeof(TASK2_WORD_METADATA), 1, pfOutput) != 1){
                perror("Failed to write struct to file\n");
                fclose(pTmpFile);
                fclose(pFile);
                fclose(pfOutput);
                free(psWord);
                return 1;
            }
            if (fwrite(psWord->szWord, iWordSize + 1, 1, pfOutput) != 1){
                perror("Failed to write word to file\n");
                fclose(pTmpFile);
                fclose(pFile);
                fclose(pfOutput);
                free(psWord);
                return 1;
            }


            fseek(pFile, 0, SEEK_CUR);
            free(psWord);
            psWord = 0;
        } else {
            break;
        }
    }

    fclose(pfOutput);
    fclose(pTmpFile);
    fclose(pFile);

    readBinary();
    return 0;
}

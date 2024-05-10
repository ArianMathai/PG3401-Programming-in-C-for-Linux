#include "include/oppgave_5.h"
#include <pthread.h>


void *threadFunction(void *args){

    int clientSocket_fd = *(int *)args;
    TASK5_CONNECT_USR *psClientConfig = NULL;
    psClientConfig = malloc(sizeof (TASK5_CONNECT_USR));
    if (psClientConfig == NULL){
        printf("malloc of psClientConfig failed\n");
        return NULL;
    }
    int iConfigSize = sizeof (TASK5_CONNECT_USR);
    int iReceivedHandshake = 1;
    char *pszMessage = NULL;

    while (1){
        int iHandshake = recv(clientSocket_fd, psClientConfig, iConfigSize, MSG_DONTWAIT);
        if (iHandshake < 0){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                continue;
            } else {
                printf("Failed to receive\n");
                break;
            }
        }
        if (iHandshake < iConfigSize){
            printf("Failed on receiving connection header\n");
            break;
        }
        else {
            char *ipAddressString = inet_ntoa((struct in_addr){
                psClientConfig->iIpAddress
            });
            if (ipAddressString == NULL) {
                perror("inet_ntoa fail");
            } else {
                printf("Client IP Address: %s\n", ipAddressString);
            }
            iReceivedHandshake = 0;
            printf("ClientConfig = %d, %d, %s, %d\n", psClientConfig->iMessageSize, psClientConfig->iMagicNumber, ipAddressString, psClientConfig->iPhoneNumber);
            break;
        }
    }

    int iMagicNum = 0;
    int iMessageSize = 0;
    char szHeaderBuf[8];
    char szBuf[128];
    int iReceivedBytes = 0;
    int iErrorFlag = 0;
    int iTotalBytesReceived = 0;
    int iExitFlag = 0;
    int iHeaderBytesReceived = 0;

    if (iReceivedHandshake == 0){
        while (1) {
            if (iErrorFlag == 1) {
                break;
            }
            while (iHeaderBytesReceived < 8) {
                iReceivedBytes = recv(clientSocket_fd, szHeaderBuf + iHeaderBytesReceived, 8 - iHeaderBytesReceived, MSG_DONTWAIT);
                if (iReceivedBytes < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        continue;
                    } else {
                        printf("Failed to receive\n");
                        break;
                    }
                }
                if (iReceivedBytes > 0) {
                    iHeaderBytesReceived += iReceivedBytes;
                    iTotalBytesReceived += iReceivedBytes;
                }
            } // End while receive header

            if (iHeaderBytesReceived == 8) {
                memcpy(&iMagicNum, szHeaderBuf, sizeof(iMagicNum));
                iMagicNum = ntohl(iMagicNum);
                memcpy(&iMessageSize, szHeaderBuf + 4, sizeof(iMessageSize));
                if (iMagicNum != psClientConfig->iMagicNumber) {
                    printf("Magic number does not match! You are using wrong protocol!\n");
                    iErrorFlag = 1;
                } else {
                    pszMessage = malloc(iMessageSize + 1);
                    if (pszMessage == NULL) {
                        printf("Failed to malloc message!\n");
                        iErrorFlag = 1;
                    } else {
                        memset(pszMessage, 0, iMessageSize + 1);
                    }
                }
            } else {
                printf("Did not receive full message header with magic number and size\n");
                iErrorFlag = 1;
            }

            if (iErrorFlag == 1) {
                perror("ErrorFlag 1");
                break;
            }

            char *pszTemp = pszMessage;
            while (iTotalBytesReceived < iMessageSize) {
                iReceivedBytes = recv(clientSocket_fd, szBuf, sizeof(szBuf), MSG_DONTWAIT);

                if (iReceivedBytes < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        continue;
                    } else {
                        iErrorFlag = 1;
                        printf("Failed to receive\n");
                        break;
                    }
                } else if (iReceivedBytes == 0) {
                    printf("Received 0 bytes\n");
                    iErrorFlag = 1;
                    break;
                } else {
                    if (strncmp(szBuf, "--EXIT--", strlen("--EXIT--")) == 0){
                        printf("Closing client %d\n", psClientConfig->iPhoneNumber);
                        iExitFlag = 1;
                        break;
                    }
                    memcpy(pszTemp, szBuf, iReceivedBytes);
                    if (pszTemp == NULL){
                        iErrorFlag = 1;
                        break;
                    }
                    pszTemp += iReceivedBytes;
                    iTotalBytesReceived += iReceivedBytes;
                }
            } // end while receiving message
            if (iExitFlag == 1){
                break;
            }
            if (iErrorFlag == 0 && pszMessage != NULL) {
                pszMessage[iTotalBytesReceived] = '\0';
                printf("Number %d: %s\n", psClientConfig->iPhoneNumber, pszMessage); // Printing the message from client
                free(pszMessage);
                pszMessage = NULL;
                iHeaderBytesReceived = 0;
                iTotalBytesReceived = 0;
            } else {
                printf("ErrorFlag is 1! Could not read message\n");
            }
        }// End outer while loop
    }
    if (pszMessage != NULL){
        free(pszMessage);
    }
    free(psClientConfig);
    close(clientSocket_fd);

    pthread_exit(NULL);
}


void StartServer(int iPort) {

    int socket_fd = -1;
    int iRc = 1;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_id[MAX_CLIENTS];
    int num_clients = 0;

    struct sockaddr_in saAddr = {0};
    memset(&saAddr, 0, sizeof(saAddr));
    saAddr.sin_family = AF_INET;
    saAddr.sin_port = htons(iPort);
    saAddr.sin_addr.s_addr = INADDR_ANY;

    iRc = bind(socket_fd, (struct sockaddr *)&saAddr, sizeof(saAddr));
    if (iRc < 0) {
        perror("Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }

    iRc = listen(socket_fd, MAX_CLIENTS);
    if (iRc < 0) {
        perror("Failed when listening\n");
        exit(EXIT_FAILURE);
    }

    while (num_clients < MAX_CLIENTS) {
        struct sockaddr_in saClientAddr = {0};
        socklen_t clientAddrLen = sizeof(saClientAddr);

        int clientSocket_fd = accept(socket_fd, (struct sockaddr *)&saClientAddr, &clientAddrLen);
        if (clientSocket_fd < 0) {
            perror("Error accepting client connection\n");
            continue;
        }

        if (pthread_create(&thread_id[num_clients], NULL, threadFunction, &clientSocket_fd) != 0) {
            perror("Error creating thread");
            close(clientSocket_fd);
            continue;
        }
        num_clients++;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (pthread_join(thread_id[i], NULL) != 0) {
            perror("Error joining thread");
            exit(EXIT_FAILURE);
        }
    }
    close(socket_fd);

}
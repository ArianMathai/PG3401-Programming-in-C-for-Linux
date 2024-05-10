#include "include/oppgave_5.h"


void StartClient(int iPort, char *ipAddress, int iPhoneNum){

    int socket_fd = -1;
    int iRc = 1;
    char *szBuffer = NULL;

    struct sockaddr_in saAddr = {0};

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        perror("Failed to create socket.\r\n");
        exit(EXIT_FAILURE);
    }
    saAddr.sin_family = AF_INET;
    saAddr.sin_port = htons(iPort);
    saAddr.sin_addr.s_addr = inet_addr(ipAddress);

    if (connect(socket_fd, (struct sockaddr *)&saAddr, sizeof(saAddr)) < 0) {
        perror("Error connecting.\r\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connection to server successful\n");

    while (1) {
        TASK5_CONNECT_USR *psConnectMessage = malloc(sizeof (TASK5_CONNECT_USR));
        if (psConnectMessage == NULL){
            perror("Failed to malloc connect message");
            return;
        }
        else{
            psConnectMessage->iMagicNumber = 1; // Our protocol number
            psConnectMessage->iPhoneNumber = iPhoneNum;
            psConnectMessage->iIpAddress = inet_addr(ipAddress);
            psConnectMessage->iMessageSize = sizeof (TASK5_CONNECT_USR);
        }
        if (sendHeader(psConnectMessage, socket_fd) != 0){
            free(psConnectMessage);
            break;
        }
        free(psConnectMessage);

        while(1){
            printf("Write --EXIT-- to exit\n");
            szBuffer = getDynamicInput(); // remember to free szBuffer!!!
            if (szBuffer == NULL){
                printf("Failed to get input from user\n");
                break;
            }
            int iBufferLength = strlen(szBuffer);
            TASK5_SENDMESSAGE *psMessage = malloc(sizeof (TASK5_SENDMESSAGE) + iBufferLength + 1);
            if (psMessage == NULL){
                printf("Failed to malloc message\n");
                free(szBuffer);
                break;
            }
            psMessage->iMagicNumber = htonl(1);
            psMessage->iMessageSize = sizeof(TASK5_SENDMESSAGE) + iBufferLength;
            strncpy(psMessage->szMessage, szBuffer, iBufferLength);
            psMessage->szMessage[iBufferLength] = '\0';
            free(szBuffer);
            iRc = send(socket_fd, psMessage, sizeof(TASK5_SENDMESSAGE) + iBufferLength, MSG_DONTWAIT);
            if (iRc == -1) {
                perror("send failed");
                free(psMessage);
                break;
            }
            if (strncmp(psMessage->szMessage, "--EXIT--", strlen("--EXIT--")) == 0){
                printf("Closing client\n");
                free(psMessage);
                break;
            }
            free(psMessage);
        }
        break;
    }
    if (close(socket_fd) < 0) {
        perror("Error closing socket");
    }
}

int sendHeader(TASK5_CONNECT_USR *psHeader, int socket_fd){
    int iRc = send(socket_fd, psHeader, sizeof(*psHeader), 0);

    if (iRc < 0) {
        printf("Failed to send header through socket.\r\n");
        return 1;
    }
    return 0;
}

char *getDynamicInput() {
    int bufferSize = 128;
    int currentPosition = 0;
    char *pszBuffer = malloc(bufferSize);
    char c;
    if (!pszBuffer) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    printf("Enter message:\n");
    while ((c = fgetc(stdin)) != '\n' && c != EOF) {
        pszBuffer[currentPosition++] = c;
        if (currentPosition == bufferSize - 1) {
            size_t newBufferSize = bufferSize * 2;
            char *pszNewBuffer = malloc(newBufferSize);

            if (!pszNewBuffer) {
                printf("Memory allocation failed\n");
                free(pszBuffer);
                return NULL;
            }

            memcpy(pszNewBuffer, pszBuffer, currentPosition);
            free(pszBuffer);
            pszBuffer = pszNewBuffer;
            bufferSize = newBufferSize;
        }
    }
    pszBuffer[currentPosition] = '\0';

    return pszBuffer;
}

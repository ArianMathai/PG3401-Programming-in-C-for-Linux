

#ifndef C_I_LINUX_OPPGAVE_5_H
#define C_I_LINUX_OPPGAVE_5_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 2
#define MAX_BUFFER_SIZE 1024
#define MAX_IP_SIZE 16

typedef struct _TASK5_CONNECT_USR {
    int iMagicNumber; // Set to a fixed protocol id no
    int iMessageSize; // Set to sizeof(TASK5_CONNECT_USR)
    int iIpAddress;   // Set to local client IP address
    int iPhoneNumber; // Set to «phone number»
} TASK5_CONNECT_USR;

#pragma pack(1)
typedef struct _TASK5_SENDMESSAGE {
    int iMagicNumber; // Set to a fixed protocol id no
    int iMessageSize; // Set to sizeof(TASK5_SENDMESSAGE)
    // + strlen(szMessage)
    char szMessage[]; // A variable length message
} TASK5_SENDMESSAGE;
#pragma pack()

void StartClient(int iPort, char *ipAddress, int iPhoneNum);
void StartServer(int iPort);
char *getDynamicInput();
int sendHeader(TASK5_CONNECT_USR *psHeader, int socket_fd);


#endif //C_I_LINUX_OPPGAVE_5_H

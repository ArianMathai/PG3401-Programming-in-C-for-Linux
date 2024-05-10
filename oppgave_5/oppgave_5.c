#include "include/oppgave_5.h"
#include <limits.h>

// Client server simple message protocol. Up to two clients can talk to the server.
//Start application with params on format:

// for server    =    ./oppgave_5 -listen -port 8080
// for client    =    ./oppgave_5 -server 127.0.0.1 -port 1050 -tlf 5678


int isValidIp(const char *ipAddress) {
    struct in_addr addr;
    int result = inet_pton(AF_INET, ipAddress, &addr);
    return result == 1;
}

int main(int iArgc, char *pszArgv[]) {

    if (iArgc < 4 || iArgc > 7) {
        printf("Invalid number of args arguments.\r\n");
        return 1;
    }
    char ip_address[16];
    int iPort = 0;
    int iTlfNum = 0;

    if ((strncmp(pszArgv[1], "-listen", strlen("-listen")) == 0)) {
        if ((strncmp(pszArgv[2], "-port", strlen("-port")) == 0)) {
            iPort = atoi(pszArgv[3]);
            if (iPort < 1024 || iPort > 65535) {
                printf("Port %d invalid. Choose port between 1024-65535\n", iPort);
                return 1;
            }
            printf("Starting server at port %d...\n", iPort);
            StartServer(iPort);
        } else {
            perror("Start application with params on format:\n./oppgave_5 -listen -port 8080\noppgave_5 -server 127.0.0.1 -port 1050 -tlf 5678");
            return 1;
        }
    } else if ((strncmp(pszArgv[1], "-server", strlen("-server")) == 0)) {
        strncpy(ip_address, pszArgv[2], sizeof(ip_address) - 1);
        ip_address[sizeof(ip_address) - 1] = '\0';
        if (isValidIp(ip_address) != 1){
            printf("Not valid ip address\n");
            return 1;
        } else {
            if ((strncmp(pszArgv[3], "-port", strlen("-port")) == 0)) {
                iPort = atoi(pszArgv[4]);
                if (iPort < 1024 || iPort > 65535) {
                    printf("Port %d invalid. Choose port between 1024-65535\n", iPort);
                    return 1;
                }
                if ((strncmp(pszArgv[5], "-tlf", strlen("-tlf")) == 0)) {
                    iTlfNum = atoi(pszArgv[6]);
                    if (iTlfNum > INT_MAX || iTlfNum < INT_MIN) {
                        printf("Phone number %d is out of range.\n", iTlfNum);
                        return 1;
                    }
                } else {
                    printf("param -tlf is needed\n");
                    return 1;
                }
                printf("Starting client at port %d, ipaddr %s, phonenum %d\n", iPort, ip_address, iTlfNum);
                StartClient(iPort, ip_address, iTlfNum);
            }
        }
    } else {
        perror("Start application with params on format:\n./oppgave_5 -listen -port 8080\noppgave_5 -server 127.0.0.1 -port 1050 -tlf 5678");
        return 1;
    }
}
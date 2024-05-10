#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "include/oppgave_3.h"

void toLowerString(char *szString){
    while (*szString) {
        *szString = tolower(*szString);
        szString++;
    }
}

void searchReservation(RESERVATIONLIST *psResList){
    if (psResList->pHead == NULL){
        printf("No reservations! Returning to main menu\n");
        return;
    }
    char userInput[MAX_NAME_LENGTH];
    printf("Enter reservation name:\n");
    if (fgets(userInput, sizeof(userInput), stdin) == NULL){
        perror("Failed on getting reservation name from user!\n");
        return;
    }
    int len = strlen(userInput);
    if (len > 0 && userInput[len - 1] == '\n') {
        userInput[len - 1] = '\0';
    }
    toLowerString(userInput);
    TABLERESERVATION *psCur = psResList->pHead;
    while(psCur != NULL){
        char tmp[MAX_NAME_LENGTH];
        strncpy(tmp, psCur->szReservationName, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';
        toLowerString(tmp);
        if (strncmp(tmp, userInput, len) == 0){
            printf("*******************************************\n");
            printf("Name = %s\nTable number = %d\nTime = %08d\n", psCur->szReservationName, psCur->iTableNumber, psCur->iResTime);
            printf("*******************************************\n");
            break;
        }
        psCur = psCur->psNext;
    }
}
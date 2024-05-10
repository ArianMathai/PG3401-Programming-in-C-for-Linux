#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave_3.h"

int deleteReservation(TABLERESERVATION *psRes, RESERVATIONLIST *psResList){
    if (psRes == NULL){
        printf("Reservation to be deleted is NULL\n");
        return 1;
    }
    TABLERESERVATION *psResToDelete = psRes;
    if (psResToDelete->psOrderHead != NULL){ // Delete orders if they exist
        TABLEORDERS *psOrder = psResToDelete->psOrderHead;
        while (psOrder != NULL) {
            TABLEORDERS *temp = psOrder;
            psOrder = psOrder->psNext;
            free(temp);
        }
        psResToDelete->psOrderHead = NULL;
    }
    if (psResToDelete->psPrev == NULL) { // psRes is the head
        if (psResToDelete->psNext == NULL){ // Only one element in list
            psResList->pHead = NULL;
            psResList->pTail = NULL;
        }
        else {
            psResToDelete->psNext->psPrev = NULL;
            psResList->pHead = psResToDelete->psNext;
        }
    }
    else if (psResToDelete->psNext == NULL){ // if psRes is the tail
        psResToDelete->psPrev->psNext = NULL;
        psResList->pTail = psResToDelete->psPrev;
    }
    else {
        psResToDelete->psPrev->psNext = psResToDelete->psNext;
        psResToDelete->psNext->psPrev = psResToDelete->psPrev;
    }
    free(psResToDelete);
    return 0;
}

void removeReservation(RESERVATIONLIST *psResList){
    if (psResList->pHead == NULL){
        printf("Reservation list is empty. Nothing to delete!\n");
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
        zero_terminated_strncpy(tmp, psCur->szReservationName, sizeof(tmp));
        toLowerString(tmp);
        if (strncmp(tmp, userInput, MAX_NAME_LENGTH) == 0){
            break;
        }
        else{
            psCur = psCur->psNext;
        }
    }
    if (psCur != NULL) {
        int isDeleted = deleteReservation(psCur, psResList);
        if (isDeleted == 0){
            printf("Deleted reservation with name %s\n", userInput);
        }
        else {
            printf("Could not delete reservation for %s\n", userInput);
        }
    } else {
        printf("No reservation found with name %s\n", userInput);
    }
}
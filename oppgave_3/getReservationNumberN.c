#include <stdio.h>
#include <string.h>
#include "include/oppgave_3.h"

void getReservationNumberN(RESERVATIONLIST *psResList){
    if (psResList->pHead == NULL){
        printf("No reservations in list. Can find reservation\n");
        return;
    }
    printf("Enter index of reservation you want to print:\n");
    int n = getIntFromUser();
    if (n < 1){
        printf("Indexing starts at 1, %d is not valid input\nReturning to main menu\n", n);
        return;
    }
    TABLERESERVATION *psCurRes = psResList->pHead;

    int iCount = 1;
    while (iCount != n){
        if (psCurRes == NULL){
            printf("Reservation list does not contain index %d\n", n);
            return;
        }
        iCount++;
        psCurRes = psCurRes->psNext;
    }
    printReservation(psCurRes);
}



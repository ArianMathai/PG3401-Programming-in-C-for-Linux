#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave_3.h"

// Table reservation system
// Run with ./oppgave_3

int main(int iArgc, char *argv[]){

    if (iArgc != 1){
        perror("Run program with no args\n");
        return 1;
    }

    char option;
    int input;

    RESERVATIONLIST *psReservationlist = malloc(sizeof (RESERVATIONLIST));
    if (psReservationlist == NULL){
        perror("Failed on malloc of RESERVATIONLIST\n");
        return 1;
    }
    else {
        psReservationlist->pTail = NULL;
        psReservationlist->pHead = NULL;
    }

    do {
        printf("\n\t\tMENU\n");
        printf("\t--------------------------------\n");
        printf("\t1. Make reservation\n");
        printf("\t2. Get reservation based on index (indexing starts at 1)\n");
        printf("\t3. Search reservation\n");
        printf("\t4. Delete reservation\n");
        printf("\t5. Add food or drinks\n");
        printf("\t6. Print receipt for table\n");
        printf("\t7. Print receipt for person\n");
        printf("\t8. Print all reservations\n");
        printf("\t9. Exit\n");
        printf("\t--------------------------------\n");
        printf("Enter your choice (1-9): \n");
        input = getchar();
        while (getchar() != '\n') {

        }
        if (input >= '1' && input <= '9') {
            option = input;

            switch (option) {
                case '1':
                    addReservation(psReservationlist);
                    break;
                case '2':
                    getReservationNumberN(psReservationlist);
                    break;
                case '3':
                    searchReservation(psReservationlist);
                    break;
                case '4':
                    removeReservation(psReservationlist);
                    break;
                case '5':
                    addFood(psReservationlist);
                    break;
                case '6':
                    printReceiptForTable(psReservationlist);
                    break;
                case '7':
                    printReceiptForPerson(psReservationlist);
                    break;
                case '8':
                    printReservationList(psReservationlist);
                    break;
                case '9':
                    printf("Exiting program\n");
                    break;
            }

        } else {
            printf("Invalid input. Please enter a number between 1 and 9.\n");
        }
    } while (option != '9');

    deleteAllReservations(psReservationlist);
    free(psReservationlist);

    return 0;

}

void deleteAllReservations(RESERVATIONLIST *psResList) {
    TABLERESERVATION *psCurRes = psResList->pHead;
    TABLERESERVATION *psNextRes = NULL;
    while (psCurRes != NULL) {
        psNextRes = psCurRes->psNext;

        TABLEORDERS *psCurOrder = psCurRes->psOrderHead;
        TABLEORDERS *psNextOrder;
        while (psCurOrder != NULL) {
            psNextOrder = psCurOrder->psNext;
            free(psCurOrder);
            psCurOrder = psNextOrder;
        }
        free(psCurRes);
        psCurRes = psNextRes;
    }
    psResList->pHead = NULL;
    psResList->pTail = NULL;
}

void printReservation(TABLERESERVATION *psRes){
    if (psRes == NULL){
        printf("Reservation is NULL. Not able to print\n");
    }
    else {
        printf ("********************\n");
        printf("name = %s\nnumGuests = %d\niTime = %08d\nTableNum = %d\n", psRes->szReservationName, psRes->iSeats, psRes->iResTime, psRes->iTableNumber);
        if (psRes->psOrderHead != NULL){
            TABLEORDERS *psCurOrder = psRes->psOrderHead;
            while (psCurOrder != NULL){
                printf("----------------------\n");
                printf("Name = %s\n", psCurOrder->szBuyerName);
                printf("Food = %s\n", psCurOrder->szFoodDescription);
                printf("Price = %d kr\n", psCurOrder->iPrice);
                psCurOrder = psCurOrder->psNext;
                printf("----------------------\n");
            }
        }
        printf ("********************\n");
    }

}

void printReservationList (RESERVATIONLIST *psResList){
    if (psResList->pHead == NULL){
        printf("No reservations! List is empty!\n");
        return;
    }
    TABLERESERVATION *psCur = psResList->pHead;
    printf ("********************\n");
    while (psCur != NULL) {
        printReservation(psCur);
        psCur = psCur->psNext;
    }
    printf ("********************\n");
}
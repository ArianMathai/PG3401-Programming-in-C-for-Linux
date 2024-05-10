#include <stdio.h>
#include <string.h>
#include "include/oppgave_3.h"

void printReceiptForPerson(RESERVATIONLIST *psResList){

    if (psResList == NULL){
        printf("Reservation list is empty\n");
        return;
    }
    int iTableNum = 0;
    while (1){
        printf("Enter table number for receipt\n");
        iTableNum = getIntFromUser();
        if (iTableNum == -1){
            perror("Error getting tablenum from user\n");
            return;
        }
        if (iTableNum < 1 || iTableNum > 70){
            printf("Choose a table number between 1 and 70!\n");
            continue;
        }
        else {
            break;
        }
    }
    TABLERESERVATION *psRes = getReservationByTableNumber(psResList, iTableNum);
    if (psRes == NULL){
        return;
    }
    char userInput[MAX_NAME_LENGTH];
    printf("Enter your name of person you want receipt for:\n");
    if (fgets(userInput, sizeof(userInput), stdin) == NULL){
        perror("Failed on getting name from user\n");
        return;
    }
    int len = strlen(userInput);
    if (len > 0 && userInput[len - 1] == '\n') {
        userInput[len - 1] = '\0';
    }
    toLowerString(userInput);
    int iTotalSum = 0;

    TABLERESERVATION *psCurRes = psResList->pHead;
    while (psCurRes != NULL) {
        TABLEORDERS *psCurOrder = psCurRes->psOrderHead;
        if (psCurOrder == NULL){
            psCurRes = psCurRes->psNext;
            continue;
        }
        while (psCurOrder != NULL) {
            toLowerString(psCurOrder->szBuyerName);
            if (strncmp(psCurOrder->szBuyerName, userInput, MAX_NAME_LENGTH) == 0) {
                printf("**********************\n");
                printf("Buyer Name: %s\n", psCurOrder->szBuyerName);
                printf("Food Description: %s\n", psCurOrder->szFoodDescription);
                printf("Price: %d\n", psCurOrder->iPrice);
                iTotalSum += psCurOrder->iPrice;
            }
            psCurOrder = psCurOrder->psNext;
        }
        psCurRes = psCurRes->psNext;
    }

    if (iTotalSum > 0) {
        printf("************************************\n");
        printf("TOTAL PRICE FOR %s: %d\n", userInput, iTotalSum);
        printf("************************************\n");
    } else {
        printf("No orders found for %s\n", userInput);
    }
}

void printReceiptForTable(RESERVATIONLIST *psResList){

    if (psResList == NULL){
        printf("Reservation list is empty\n");
        return;
    }
    int iTableNum = 0;
    while (1){
        printf("Enter table number for receipt\n");
        iTableNum = getIntFromUser();
        if (iTableNum == -1){
            perror("Error getting tablenum from user\n");
            return;
        }
        if (iTableNum < 1 || iTableNum > 70){
            printf("Choose a table number between 1 and 70!\n");
            continue;
        }
        else {
            break;
        }
    }
    TABLERESERVATION *psRes = getReservationByTableNumber(psResList, iTableNum);
    if (psRes == NULL){ // Either no reservations, or tableNum not found on reservations
        return;
    }
    printf("Receipt for Table %d\n", iTableNum);
    TABLEORDERS *psCurOrder = psRes->psOrderHead;
    int iTotalSum = 0;
    if (psCurOrder != NULL){
        while (psCurOrder != NULL) {
            printf("**********************\n");
            printf("%s\n%s\n%d\n", psCurOrder->szBuyerName, psCurOrder->szFoodDescription, psCurOrder->iPrice);
            iTotalSum += psCurOrder->iPrice;
            psCurOrder = psCurOrder->psNext;
        }
        printf("************************************\n");
        printf("TOTAL PRICE FOR TABLE: %d\n", iTotalSum);
        printf("************************************\n");
    }
    else {
        printf("No orders for this table\n");
    }
}
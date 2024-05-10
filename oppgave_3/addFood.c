#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave_3.h"

char *getDynamicInput() {
    int bufferSize = 128;
    int currentPosition = 0;
    char *pszBuffer = malloc(bufferSize);
    char c;
    if (!pszBuffer) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    while ((c = fgetc(stdin)) != '\n' && c != EOF) {
        pszBuffer[currentPosition++] = c;
        if (currentPosition == bufferSize - 1) {
            int newBufferSize = bufferSize * 2;
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
TABLERESERVATION* getReservationByTableNumber(RESERVATIONLIST *psResList, int iTableNumber) {
    if (psResList == NULL) {
        printf("Reservation list is empty\n");
        return NULL;
    }
    TABLERESERVATION *psCur = psResList->pHead;
    while (psCur != NULL) {
        if (psCur->iTableNumber == iTableNumber) {
            return psCur;
        }
        psCur = psCur->psNext;
    }
    printf("No reservation found for table number %d\n", iTableNumber);
    return NULL;
}

void createOrder(TABLEORDERS **ppsOrdersHead, char *pszName, int iPrice, char *pszFoodDesc, int iDescSize) {
    TABLEORDERS *newOrder = (TABLEORDERS *)malloc(sizeof(TABLEORDERS) + iDescSize + 1);
    if (newOrder == NULL) {
        perror("Memory allocation failed\n");
        return;
    }
    newOrder->psNext = NULL;
    newOrder->iPrice = iPrice;
    strncpy(newOrder->szBuyerName, pszName, MAX_NAME_LENGTH - 1);
    newOrder->szBuyerName[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(newOrder->szFoodDescription, pszFoodDesc, iDescSize);
    newOrder->szFoodDescription[iDescSize] = '\0';
    if (*ppsOrdersHead == NULL) {
        *ppsOrdersHead = newOrder; // Set the new order as the head if no orders exist
    } else {
        TABLEORDERS *psCur = *ppsOrdersHead;
        while (psCur->psNext != NULL) {
            psCur = psCur->psNext;
        }
        psCur->psNext = newOrder;
    }
}

void addFood(RESERVATIONLIST *psResList){
    if (psResList == NULL){
        printf("Reservation list is empty\n");
        return;
    }
    int iTableNum = 0;
    while (1){
        printf("Enter table number you want to add order to (1-70):\n");
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

    char userInput[MAX_NAME_LENGTH];
    printf("Enter name of customer:\n");
    if (fgets(userInput, sizeof(userInput), stdin) == NULL){
        perror("Failed on getting customer name from user\n");
        return;
    }
    int len = strlen(userInput);
    if (len > 0 && userInput[len - 1] == '\n') {
        userInput[len - 1] = '\0';
    }
    printf("Enter food or drink description:\n");
    char *pszFoodDescription = NULL;

    pszFoodDescription = getDynamicInput();
    if (pszFoodDescription == NULL){
        printf("Failed on getting food description\n");
        return;
    }
    int numChars = strlen(pszFoodDescription);

    printf("Enter price of food or drink:\n");
    int iPrice = getIntFromUser();
    if (iPrice == -1){
        perror("Error getting price\n");
        return;
    }

    createOrder(&(psRes->psOrderHead), userInput, iPrice, pszFoodDescription, numChars);

    free(pszFoodDescription);

}
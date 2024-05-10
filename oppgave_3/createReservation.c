#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave_3.h"

#define MAX_RESERVATION_TIME_DIFFERENCE 3 * 60 // 3 hours

int difference(int a, int b) {
    if (a >= b) {
        return a - b;
    } else {
        return b - a;
    }
}

int hasActiveReservation(RESERVATIONLIST *psResList, int iTableNum, int iCurTime) {
    TABLERESERVATION *psCurRes = psResList->pHead;
    while (psCurRes != NULL) {
        if (psCurRes->iTableNumber == iTableNum && difference(psCurRes->iResTime, iCurTime) <= MAX_RESERVATION_TIME_DIFFERENCE) {
            return 1;
        }
        psCurRes = psCurRes->psNext;
    }
    return 0;
}

// I had an issue when using strncpy, so i made my own copy function
int zero_terminated_strncpy(char *dest, const char *src, int iSize) {
    if (iSize < 1){
        perror("iSize is less than 1 when copying string\n");
        return 1;
    }
    int i;
    for (i = 0; i < iSize - 1 && src[i] != '\0' && src[i] != '\n'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int insertInListSorted(RESERVATIONLIST *psResList, TABLERESERVATION *psNewRes){
    int iRc = 1;
    if(psNewRes == NULL){
        perror("Pointer to reservation struct is NULL\n");
        return iRc;
    }
    if (psResList->pHead == NULL){ // if reservation list is empty
        psResList->pHead = psNewRes;
        psResList->pTail = psNewRes;
        psNewRes->psNext = NULL;
        psNewRes->psPrev = NULL;
        iRc = 0;
        return iRc;
    }
    TABLERESERVATION *psCurRes = psResList->pHead;
    if (strcmp(psResList->pHead->szReservationName, psNewRes->szReservationName) > 0){ // if new reservation should go first in list
        psNewRes->psNext = psResList->pHead;
        psResList->pHead = psNewRes;
        psNewRes->psPrev = NULL;
        iRc = 0;
        return iRc;
    }
    while(psCurRes){
        if (psCurRes->psNext == NULL){ // We reached end of list and newRes should be inserted here
            psCurRes->psNext = psNewRes;
            psNewRes->psPrev = psCurRes;
            psNewRes->psNext = NULL;
            psResList->pTail = psNewRes;
            iRc = 0;
            break;
        }
        int iComp = strcmp(psCurRes->psNext->szReservationName, psNewRes->szReservationName);
        if (iComp >= 0){ // if currentRes is greater than or equal to newRes
            // Put before current
            TABLERESERVATION *psTmp = psCurRes->psNext;
            psCurRes->psNext = psNewRes;
            psNewRes->psPrev = psCurRes;
            psNewRes->psNext = psTmp;
            psTmp->psPrev = psNewRes;
            iRc = 0;
            break;
        }
        else{
            // newRes is greater than currentRes
            // go next
            psCurRes = psCurRes->psNext;
        }
    }
    return iRc;
}

int getIntFromUser() {
    char input[64];
    int number;

    while (1){
        if (fgets(input, sizeof(input), stdin) == NULL){
            perror("Error getting int from user\n");
            return -1;
        }
        if (sscanf(input, "%d", &number) == 1) {
            break;
        } else {
            printf("Invalid input. Please enter an integer.\n");
        }
    }
    return number;
}
int getTimeFromUser() {
    char buffer[9];
    int time;
    int month, day, hour, minute;

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input.\n");
            return -1;
        }
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (strlen(buffer) != 8) {
            fprintf(stderr, "Invalid time format. Please enter time as MMDDHHMM.\n");
            continue;
        }
        if (sscanf(buffer, "%2d%2d%2d%2d", &month, &day, &hour, &minute) != 4 ||
            month < 1 || month > 12 || day < 1 || day > 31 ||
            hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            printf("Invalid time entered.\n");
            continue;
        } else {
            break;
        }
    }
    time = month * 1000000 + day * 10000 + hour * 100 + minute;
    return time;
}

TABLERESERVATION* createReservation(int iTableNum, int iNumOfGuests, int iResTime, char *szResName) {
    TABLERESERVATION *psThis = malloc(sizeof(TABLERESERVATION));
    if (psThis != NULL) {
        memset(psThis, 0, sizeof(TABLERESERVATION));
        psThis->iSeats = iNumOfGuests;
        psThis->iTableNumber = iTableNum;
        psThis->iResTime = iResTime;
        psThis->psOrderHead = NULL;
        zero_terminated_strncpy(psThis->szReservationName, szResName, MAX_NAME_LENGTH);
    }
    return psThis;
}
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int addReservation(RESERVATIONLIST *psResList){
    // get info on reservation
    char userInput[MAX_NAME_LENGTH];
    printf("Enter your reservation name:\n");
    if (fgets(userInput, sizeof(userInput), stdin) == NULL){
        perror("Failed on getting reservation name from user\n");
        return 1;
    }
    int len = strlen(userInput);
    if (len > 0 && userInput[len - 1] == '\n') {
        userInput[len - 1] = '\0';
    }
    printf("Enter number of guests:\n");
    int iNumOfGuests = 0;
    while(1){
        iNumOfGuests = getIntFromUser();
        if (iNumOfGuests == -1){
            perror("Error getting number of guest from user\n");
            return 1;
        }
        if (iNumOfGuests > 0 && iNumOfGuests < 51){
            break;
        }
        else {
            printf("Number of guests must be between 1 - 50\n");
        }
    }

    printf("Enter time of reservation on format(MMDDHHMM):\n");
    int iTime = getTimeFromUser();
    if (iTime == -1){
        perror("Error getting time from user\n");
        return 1;
    }
    clearInputBuffer();
    int iTableNum = 0;
    while (1){
        printf("Enter tablenumber you want to reserve (1-70):\n");
        iTableNum = getIntFromUser();
        if (iTableNum == -1){
            perror("Error getting tablenum from user\n");
            return 1;
        }
        if (iTableNum < 1 || iTableNum > 70){
            printf("Choose a table number between 1 and 70!\n");
            continue;
        }
        int hasActiveRes = hasActiveReservation(psResList, iTableNum, iTime);
        if (hasActiveRes == 0){
            break;
        }
        else {
            printf("Tablenumber %d is busy during this time\n", iTableNum);
        }
    }
    // Create reservation
    TABLERESERVATION *psRes = createReservation(iTableNum, iNumOfGuests, iTime, userInput);
    if (psRes == NULL){
        perror("Failed to create reservation. Res pointer is NULL\n");
        return 1;
    }
    if (insertInListSorted(psResList, psRes) != 0){
        perror("Failed to insert reservation in list\n");
        return 1;
    }

    return 0;
}
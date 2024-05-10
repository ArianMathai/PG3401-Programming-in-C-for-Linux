

#ifndef C_I_LINUX_OPPGAVE_3_H
#define C_I_LINUX_OPPGAVE_3_H

#define MAX_NAME_LENGTH 256

typedef struct _TABLEORDERS {
    struct _TABLEORDERS *psNext;
    int iPrice;
    char szBuyerName[MAX_NAME_LENGTH];
    char szFoodDescription[];
} TABLEORDERS;

typedef struct _TABLERESERVATION {
    struct _TABLERESERVATION *psNext;
    struct _TABLERESERVATION *psPrev;
    int iTableNumber;
    int iSeats;
    int iResTime;
    struct _TABLEORDERS *psOrderHead;
    char szReservationName[MAX_NAME_LENGTH];
} TABLERESERVATION;

typedef struct _RESERVATIONLIST {
    struct _TABLERESERVATION *pHead;
    struct _TABLERESERVATION *pTail;
} RESERVATIONLIST;

int addReservation(RESERVATIONLIST *psResList);
void printReservationList (RESERVATIONLIST *psResList);
void deleteAllReservations(RESERVATIONLIST *psResList);
int getIntFromUser();
void printReservation(TABLERESERVATION *psRes);
void getReservationNumberN(RESERVATIONLIST *psResList);
void searchReservation(RESERVATIONLIST *psResList);
void toLowerString(char *szString);
void removeReservation(RESERVATIONLIST *psResList);
int zero_terminated_strncpy(char *dest, const char *src, int iSize);
void addFood(RESERVATIONLIST *psResList);
TABLERESERVATION* getReservationByTableNumber(RESERVATIONLIST *psResList, int iTableNumber);
void printReceiptForTable(RESERVATIONLIST *psResList);
void printReceiptForPerson(RESERVATIONLIST *psResList);


#endif //C_I_LINUX_OPPGAVE_3_H

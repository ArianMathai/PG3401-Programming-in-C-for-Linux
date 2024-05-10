

#ifndef C_I_LINUX_OPPGAVE_2_H
#define C_I_LINUX_OPPGAVE_2_H

#include <stdbool.h>

typedef struct _TASK2_WORD_METADATA {
    int iIndex; // The index of the word, first = 1
    bool bIsPalindrom;
    bool bIsHeterogram;
    bool bIsUppercase;
    bool bIsLowercase;
    bool bIsAnagram;
    bool bIsDisjoint;
    int iSize; // The length word (number of chars)
    char szWord[]; // The word written «after» the struct
} TASK2_WORD_METADATA;

#endif //C_I_LINUX_OPPGAVE_2_H

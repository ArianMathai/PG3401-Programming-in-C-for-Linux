#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// Code beautifier thang changes while loops to for loops, 3 spaces to tabs, and forces hungarian notation for char pointers

/*
 * I assume the increment statement can only be ++ or -- and is at the end of the while loop
 * I also assume the variable name is right before the while loop
*/

char* findThreeSpaces(char* szFileBuffer) {
    for (char *pCur = szFileBuffer; *pCur && *(pCur + 1) && *(pCur + 2); pCur++) {
        if (pCur[0] == ' ' && pCur[1] == ' ' && pCur[2] == ' ' && (pCur[3] != ' ' || pCur[3] == '\0')) {
            return pCur;
        }
    }
    return NULL;
}
char *findCharPointer(char *szFileBuffer) {
    char *pChar = strstr(szFileBuffer, "char");
    while (pChar != NULL) {
        char *pAfterChar = pChar + 4;
        while (*pAfterChar == ' ') {
            pAfterChar++;
        }
        if (*pAfterChar == '*') {
            return pChar;
        }
        pChar = strstr(pAfterChar, "char");
    }
    return NULL;
}

char *findWhileLoop(char* szFileBuffer){

    char *pFirstWhile = strstr(szFileBuffer, "while");
    if (pFirstWhile == NULL){
        return NULL;
    }
    return pFirstWhile;
}
char *trimWhitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}
char *findLastClosingBracket(char *pWhileLoop) {
    int iNumberOfBrackets = 0;
    char *pCur = pWhileLoop;
    char *pLastClosingBracket = NULL;

    while (*pCur) {
        if (*pCur == '{') {
            // Found an opening bracket, increase iNumberOfBrackets
            iNumberOfBrackets++;
        } else if (*pCur == '}') {
            // Found a closing bracket, decrease iNumberOfBrackets
            iNumberOfBrackets--;
            if (iNumberOfBrackets == 0) {
                pLastClosingBracket = pCur;
                break;
            }
        } else if (*pCur == '/' && *(pCur + 1) == '/') {
            // Found a single-line comment, skip to the end of the line
            pCur += 2;
            while (*pCur && *pCur != '\n') {
                pCur++;
            }
        } else if (*pCur == '/' && *(pCur + 1) == '*') {
            // Found a multi-line comment, skip to the end of the comment
            pCur += 2;
            while (*pCur && !(*pCur == '*' && *(pCur + 1) == '/')) {
                pCur++;
            }
            if (*pCur) {
                pCur += 2; // Skip past the closing '*/' of the comment
            }
        }
        // Move to the next character
        pCur++;
    }

    return pLastClosingBracket;
}
char *getVariableName(char *pVariableInit){
    char *pNameStart = NULL;
    char *pNameEnd = NULL;
    char *pCur = pVariableInit;

    while (*pCur && *pCur != ' ') { // skip past type (int, long...)
        pCur++;
    }
    while (*pCur && *pCur == ' ') { // Skip past space between type and varName
        pCur++;
    }
    pNameStart = pCur;

    while (*pCur && *pCur != ' ' && *pCur != '=' && *pCur != ';') { // Find end of varName
        pCur++;
    }
    pNameEnd = pCur;
    int iNameLength = pNameEnd - pNameStart;
    char *pszVarName = malloc(iNameLength + 1);
    if (pszVarName == NULL) {
        perror("Failed to malloc pszVarName");
        return NULL;
    }
    strncpy(pszVarName, pNameStart, iNameLength);
    pszVarName[iNameLength] = '\0';

    return pszVarName;
}
char *findIncrementOrDecrementOfVarName(char *pWhileLoop, char *pszVarName, char *pLastClosingBracket) {
    char *pCur = strstr(pWhileLoop, pszVarName);
    int varNameLen = strlen(pszVarName);

    while (pCur && pCur < pLastClosingBracket) {
        if (strncmp(pCur, pszVarName, varNameLen) == 0) {
            char *pNextChar = pCur + varNameLen;

            if (*pNextChar == '+' && *(pNextChar + 1) == '+') {
                return pCur;
            }
            if (*pNextChar == '-' && *(pNextChar + 1) == '-') {
                return pCur;
            }
        }

        pCur = strstr(pCur + 1, pszVarName);
    }

    return NULL;
}

char *generateForLoop(char *pVariableInit, char *pCondition, char *pIncOrDec, char *pLoopBody, char *pIndentation) {
    int sizeNeeded = strlen(pVariableInit) + strlen(pCondition) + strlen(pIncOrDec) + strlen(pLoopBody) + 50; // Extra space for safety
    char *pForLoop = malloc(sizeNeeded);
    if (pForLoop == NULL) {
        perror("Failed to allocate memory for for loop");
        return NULL;
    }

    sprintf(pForLoop, "for (%s; %s; %s) {\n%s\n%s}", pVariableInit, pCondition, pIncOrDec, pLoopBody, pIndentation);

    return pForLoop;
}


int main(int iArgc, char *argv[]){

    char* pFileBuffer = NULL;
    char *pForLoop = NULL;
    char *pNewFileBuffer = NULL;
    char *pszVarName = NULL;

    if (iArgc != 2){
        perror("Run program with filename as argument!");
        return 1;
    }
    char fileName[256] = {0};

    int iFileLen = strlen(argv[1]);
    if (iFileLen < 3 || iFileLen > 255){
        printf("Filname must be min 3 and max 255 chars long\n");
        return 1;
    } else {
        strncpy(fileName, argv[1], sizeof(fileName) - 1);
        fileName[sizeof(fileName) - 1] = '\0';
        //strncpy(fileName, argv[1], iFileLen);
    }

    FILE *pFile = NULL;
    pFile = fopen(fileName, "r");
    if (pFile == NULL){
        perror("Failed to open file");
        return 1;
    }

    if (fseek(pFile, 0, SEEK_END) != 0) {
        perror("Failed to seek to end of file");
        fclose(pFile);
        return 1;
    }

    long file_length = ftell(pFile);
    if (file_length == -1L) {
        perror("Failed to get file length");
        fclose(pFile);
        return 1;
    }

    printf("File length: %ld bytes\n\n", file_length);
    pFileBuffer = malloc(file_length + 1); // +1 for zero termination
    if (pFileBuffer == NULL){
        perror("Failed to malloc fileBuffer");
        fclose(pFile);
        return 1;
    }

    rewind(pFile);
    size_t readBytes = fread(pFileBuffer, 1, file_length, pFile);
    if (readBytes < file_length) {
        if (feof(pFile)) {
            printf("End of file reached.\n");
        }
        if (ferror(pFile)) {
            perror("Failed to read from file");
            free(pFileBuffer);
            fclose(pFile);
            return 1;
        }
    }
    pFileBuffer[file_length] = '\0';
    fclose(pFile);

    int newSize = 0; // The size of the buffer containing file. This will change through the program

    char *pCurrentPosition = pFileBuffer;
    char *pWhileLoop;
    while ((pWhileLoop = findWhileLoop(pCurrentPosition)) != NULL){
        if (pWhileLoop == NULL){
            printf("Did not find an occurrence of 'while'\n");
        } else {
            char *pConditionsStart = strstr(pWhileLoop, "(");
            if (pConditionsStart == NULL){
                printf("Did not find '(' after while\n");
            } else {
                char *pConditionEnd = strstr(pConditionsStart, ")");
                if (pConditionEnd == NULL){
                    printf("Did not find ')' after while\n");
                } else { // Extract condition
                    pConditionsStart++; // move past "("
                    pConditionEnd--; // mover before ")"
                    int iConditionSize = pConditionEnd - pConditionsStart + 1; // + 1?
                    char pCondition[iConditionSize + 1];
                    strncpy(pCondition, pConditionsStart, iConditionSize);
                    pCondition[iConditionSize] = '\0';
                    pConditionsStart--;
                    pConditionEnd++;
                    char *initEnd = pWhileLoop;
                    while (initEnd > pFileBuffer && *initEnd != ';') { // Find end of variable init
                        initEnd--;
                    }
                    char *initStart = initEnd;
                    while (initStart > pFileBuffer && *initStart != '\n') { // Find start of variable init
                        initStart--;
                    }
                    initStart++; // increment to char after '\n'. Now standing on the start of varInit

                    // get indentation before while loop
                    char *pLineStart = pWhileLoop;
                    while (pLineStart > pFileBuffer && *(pLineStart - 1) != '\n') {
                        pLineStart--;
                    }
                    //Keep indentation as it was
                    int iIndentationLength = pWhileLoop - pLineStart;
                    char pIndentation[iIndentationLength + 1];
                    strncpy(pIndentation, pLineStart, iIndentationLength);
                    pIndentation[iIndentationLength] = '\0';

                    // Keep the stuff that comes before for loop
                    int iStuffToKeepBeforeLoopLength = initStart - pCurrentPosition;
                    char beforeLoopStuff[iStuffToKeepBeforeLoopLength + 1];
                    strncpy(beforeLoopStuff, pFileBuffer, iStuffToKeepBeforeLoopLength);
                    beforeLoopStuff[iStuffToKeepBeforeLoopLength] = '\0';

                    int iVariableInitSize = initEnd - initStart;
                    char pVariableInit[iVariableInitSize + 1]; // We store the first part of the for loop here
                    strncpy(pVariableInit, initStart, iVariableInitSize);
                    pVariableInit[iVariableInitSize] = '\0';
                    char *pTrimmedVariableInit = trimWhitespace(pVariableInit);
                    pszVarName = getVariableName(pTrimmedVariableInit); // get variable name a
                    int iVarNameLength = strlen(pszVarName);
                    char *pLastClosingBracket = findLastClosingBracket(pWhileLoop); // find last closing bracket

                    // Keep the stuff that comes after for loop
                    char *pFileEnd = pFileBuffer + file_length; // Pointer to the end of the file buffer
                    int iStuffToKeepAfterLoopLength = pFileEnd - pLastClosingBracket;
                    char afterLoopStuff[iStuffToKeepAfterLoopLength + 1];
                    strncpy(afterLoopStuff, pLastClosingBracket + 1, iStuffToKeepAfterLoopLength);
                    afterLoopStuff[iStuffToKeepAfterLoopLength] = '\0';

                    char *pWhileBodyStart = strchr(pConditionEnd, '{') + 1; // find start of loop body
                    char *pIncOrDec = findIncrementOrDecrementOfVarName(pWhileLoop, pszVarName, pLastClosingBracket);
                    char pIncrementStatement[iVarNameLength + 3];
                    strncpy(pIncrementStatement, pIncOrDec, iVarNameLength + 2);
                    pIncrementStatement[iVarNameLength + 2] = '\0';
                    int iLoopBodySize = 0;
                    if (pIncOrDec == NULL) {
                        printf("Did not find decrement or increment statement\n");
                        iLoopBodySize = pLastClosingBracket - pWhileBodyStart;
                    } else {
                        iLoopBodySize = pIncOrDec - pWhileBodyStart - 1;
                        char pLoopBody[iLoopBodySize + 1];
                        strncpy(pLoopBody, pWhileBodyStart, iLoopBodySize);
                        pLoopBody[iLoopBodySize] = '\0';
                        pForLoop = generateForLoop(pTrimmedVariableInit, pCondition, pIncrementStatement, pLoopBody, pIndentation);
                        if (pForLoop == NULL) {
                            printf("Generated for loop is null\n");
                        } else {
                            newSize = iStuffToKeepBeforeLoopLength + sizeof (pIndentation) + strlen(pForLoop)+ iStuffToKeepAfterLoopLength + 1;
                            pNewFileBuffer = malloc(newSize);
                            if (pNewFileBuffer == NULL){
                                perror("Failed to malloc newFileBuffer");
                            } else {
                                snprintf(pNewFileBuffer, newSize, "%s%s%s%s", beforeLoopStuff, pIndentation, pForLoop, afterLoopStuff);
                                int iCurrentPosOffset = pCurrentPosition - pFileBuffer;
                                free(pFileBuffer);
                                pFileBuffer = pNewFileBuffer;
                                pCurrentPosition = pNewFileBuffer + iCurrentPosOffset;
                            }
                        }
                    }
                }
            }
        } // End while loop
        if (pszVarName) free(pszVarName);
        if (pForLoop) free(pForLoop);
    }
    // New while loop to add hungarian notation
    pCurrentPosition = pFileBuffer;
    char *pFirstCharPointer = NULL;
    while ((pFirstCharPointer = findCharPointer(pCurrentPosition)) != NULL){
        // Keep the stuff before variable name
        int iBeforeCharLength = pFirstCharPointer - pFileBuffer + 6;
        char beforeCharStuff[iBeforeCharLength + 1];
        strncpy(beforeCharStuff, pFileBuffer, iBeforeCharLength);
        beforeCharStuff[iBeforeCharLength] = '\0';

        char *pStartOfVarName = pFirstCharPointer + 4; // find start of varName
        if (*pStartOfVarName == '*') {
            pStartOfVarName++;
        }
        while (*pStartOfVarName == ' ') {
            pStartOfVarName++;
        }
        if (*pStartOfVarName == '*') {
            pStartOfVarName++;
        }
        char *pEndOfVarName = pStartOfVarName; // find end of varName
        while (isalnum(*pEndOfVarName) || *pEndOfVarName == '_') {
            pEndOfVarName++;
        }
        int iVarNameLength = pEndOfVarName - pStartOfVarName;
        // transform varName to hungarian notation
        char newVarName[iVarNameLength + 4];
        strncpy(newVarName, "psz", 3);
        newVarName[3] = '\0';
        strncat(newVarName, pStartOfVarName, iVarNameLength);
        if (islower(newVarName[3])) {
            newVarName[3] = toupper(newVarName[3]);
        }
        newVarName[iVarNameLength + 3] = '\0';

        char *pFileEnd = pFileBuffer + newSize; // Pointer to the end of the file buffer
        int iAfterCharLength = pFileEnd - pEndOfVarName;
        char afterCharStuff[iAfterCharLength + 1];
        strncpy(afterCharStuff, pEndOfVarName, iAfterCharLength);
        afterCharStuff[iAfterCharLength] = '\0';

        newSize = sizeof (beforeCharStuff) + sizeof (newVarName) + sizeof (afterCharStuff) + 1;
        pNewFileBuffer = malloc(newSize);
        if (pNewFileBuffer == NULL){
            perror("Failed to malloc newFileBuffer");
        } else {
            snprintf(pNewFileBuffer, newSize, "%s%s%s", beforeCharStuff, newVarName, afterCharStuff);
            pCurrentPosition = strstr(pNewFileBuffer, newVarName) + strlen(newVarName);
            free(pFileBuffer);
            pFileBuffer = pNewFileBuffer;
        }
    } // end hungarian notation while

    // Write to new file and remove three spaces for one '\t'
    FILE *pNewFile = NULL;
    char* pszNewFileName = NULL;
    char *pszDotC = strrchr(fileName, '.');
    if (pszDotC != NULL){
        pszNewFileName = malloc(sizeof (fileName) + strlen("_beautified") + 1);
        if (pszNewFileName != NULL){
            strncpy(pszNewFileName, fileName, pszDotC - fileName);
            pszNewFileName[pszDotC - fileName] = '\0';
            strcat(pszNewFileName, "_beautified.c");

            pNewFile = fopen(pszNewFileName, "w");
            if (pNewFile != NULL){

                int i = 0;
                while (i < newSize){
                    char *pThreeSpaces = findThreeSpaces(&pFileBuffer[i]);
                    if (&pFileBuffer[i] == pThreeSpaces && pThreeSpaces != NULL && pThreeSpaces - pFileBuffer < newSize){
                        fputc('\t', pNewFile);
                        i += 3;
                        newSize -= 2;
                    } else {
                        if (pFileBuffer[i] != '\0') {
                            fputc(pFileBuffer[i], pNewFile);
                        }
                        i++;
                    }
                }
            }
        }
    }


    fclose(pNewFile);
    free(pszNewFileName);
    free(pFileBuffer);

    return 0;
}






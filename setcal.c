#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define SAR_CLEAN {\
    free(sets);\
    free(relations);\
    fclose(file);\
}\

#define  MEMORY_CLEAN {\
    cleanArray(&uni.elements, uni.size);\
    free(uni.elements);\
    free(bufferLine);\
    SAR_CLEAN\
}\

typedef struct {
    char** elements;
    int size;
} universe;

//Structure for sets
typedef struct {
    char** elements;
    int position;
    int size;
} set;

//Structure for relations
typedef struct {
    char*** elements;
    int position;
    int size;
} relation;

//Cleans an array of chars
void clean(char *c) {
    int count = 0;

    while(c[count] != '\0') {
        c[count] = '\0';
        count++;
    }
}

//Checks, if end of line is reached
bool endOfLine(char c) {
    if (c == '\0' || c == '\n') {
        return true;
    }
    return false;
}

bool isBanned(char* source) {
    char* bannedWords[5] = {"U", "S", "R", "true", "false"};
    for (int i = 0; i < (int)strlen(source); i++) {
        for (int n = 0; n < 5; n++) {
            if(!strcmp(source, bannedWords[n])) {
                return true;
            }
        }
    }
    return false;
}

//Fills an array with elements from array source. Divider of elements is space
int fillArray(char*** array, char* source, int elementsCounter) {
    (*array) = malloc(sizeof *array * elementsCounter);
    for (int i = 0; i < elementsCounter; i++) {
        (*array)[i] = malloc(sizeof **array * 31);
    }
    
    char element[31];
    int positionInElement = 0;
    int positionInArray = 0;
    int start = 2;  //Position of start, because source[0] is always identificator, source[1] is ' '
    while (!endOfLine(source[start]) && positionInElement <= 30) {
        element[positionInElement] = source[start];
        element[positionInElement + 1] = '\0';
        positionInElement++;
        if (endOfLine(source[start + 1]) || source[start + 1] == ' ') {
            if(isBanned(element)) {
                fprintf(stderr, "Word %s is banned!\n", element);
                return 0;
            }
            for (int i = 0; i < positionInArray; i++) {
                if (!strcmp(element, (*array)[i])) {
                    fprintf(stderr, "Elements in set cannot repeat!\n");
                    return 0;
                }
            }
            strcpy((*array)[positionInArray], element);
            positionInArray++;                         
            clean(element);
            positionInElement = 0;
            if (endOfLine(source[start + 1])) {
                break;
            }
            if(source[start + 1] == ' ') {
                start++;
            }
        }
        start++;
    }
    if (positionInElement > 30) {
        fprintf(stderr, "Max length of argument is 30!\n");
        return 0;
    }
    return 1;
}

/*Fills an array with elements from array source. Final array must be an array of binary relations
(every element of final array is the sequence of each of the two elements in brackets from source array)*/
int fillRelation(char**** array, char* source, int relationsCounter) {
    (*array) = malloc(sizeof *array * relationsCounter);
    for(int i = 0; i < relationsCounter; i++) {
        (*array)[i] = malloc(2 * sizeof **array);
        (*array)[i][0] = malloc(31 * sizeof ***array);
        (*array)[i][1] = malloc(31 * sizeof ***array);
    }
    
    char element[31];
    int positionInElement = 0;
    int numberOfBinaryElement = 0;
    int positionInRelation = -1;
    int start = 2;
    int control = 1;
    while (!endOfLine(source[start]) && positionInElement <= 30) {
        control = 1;
        if (source[start] == '(') {
            positionInRelation++;
            numberOfBinaryElement = 0;
            control = 0;
        } else if (start == 2 && source[start] != '(') {
            fprintf(stderr, "Wrong symbols in relation!\n");
            return 0;
        } else {
            if (source[start] != '(' && source[start] != ')' && source[start] != ' ') {
                element[positionInElement] = source[start];
                element[positionInElement + 1] = '\0';
                positionInElement++;
            }
        }
        if (numberOfBinaryElement > 2) {
            fprintf(stderr, "Max amount of elements in relation is 2!");
            return 0;
        }
        if ((endOfLine(source[start + 1]) || (source[start + 1] == ' ' &&  source[start + 2] != '(')
        || source[start + 1] == ')') && control) {
            strcpy((*array)[positionInRelation][numberOfBinaryElement], element);
            if (numberOfBinaryElement) {
                int control = 0;
                char* check[2] = {(*array)[positionInRelation][numberOfBinaryElement-1], (*array)[positionInRelation][numberOfBinaryElement]};
                for (int i = 0; i < positionInRelation; i++) {
                    control = 0;
                    for (int n = 0; n < 2; n++) {
                        if(!strcmp((*array)[i][n], check[n])) {
                            control++;
                        }
                        if (control == 2) {
                            fprintf(stderr, "Binary relations cannot repeat!\n");
                            return 0;
                        }
                    }
                }
            }
            numberOfBinaryElement++;                         
            clean(element);
            positionInElement = 0;
            if (source[start + 1] == ')' && (endOfLine(source[start + 2]))) {
                break;
            }
        }
        start++;
    }
    if (positionInElement > 30) {
        fprintf(stderr, "Max length of argument is 30!\n");
        return 0;
    }
    return 1;
}

bool isElementOfArray(universe* uni, char** comparable) {
    for (int i = 0; i < uni->size; i++) {
        if(!strcmp(uni->elements[i], *comparable)) {
            return true;
        }
    }
    return false;
}

//universe* uni, relation** relations, set** sets, FILE* file, char* bufferLine
// void memoryClean(int operation, ...) {
//     free(uni->elements);
//     free(relations);
//     free(sets);
//     free(bufferLine);
//     fclose(file);
// }

void cleanArray(char*** array, int size) {
    for (int i = 0; i < size; i++) {
        free((*array)[i]);
    }
}

int main(int argc, char **argv) {
    (void) argc;

    FILE* file;
    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Cannot open file!\n");
        return 1;
    }
    int bufferSymbol;
    char* bufferLine = malloc(1);
    int symbolsCounter = 0;
    int lineCounter = 1;
    universe uni;
    set* sets = malloc(sizeof(set));
    int setCounter = 0;
    relation* relations = malloc(sizeof(relation));
    int relationCounter = 0;
    int setElementsCounter = 0;
    int relationElementsCounter = 0;

    while((bufferSymbol = fgetc(file)) != EOF) {
        char* checkBuffer = realloc(bufferLine, (symbolsCounter+2));
        if (checkBuffer == NULL) {
            fprintf(stderr, "Cannot realloc bufferLine!\n");
            return 1;
        } else {
            bufferLine = checkBuffer;
        }
        if (!endOfLine(bufferSymbol)) {
            bufferLine[symbolsCounter] = (char)bufferSymbol;
            bufferLine[symbolsCounter + 1] = '\0';
            symbolsCounter++;
            if (bufferLine[0] == 'U' || bufferLine[0] == 'S') {
                if (bufferSymbol == ' ' || (endOfLine(bufferSymbol) && (symbolsCounter - 1 == 0 || symbolsCounter - 1 == 1))) {
                    setElementsCounter++;
                }
            } else if (bufferLine[0] == 'R') {
                if (bufferSymbol == '(') {
                    relationElementsCounter++;
                }
            }
        } else {
            symbolsCounter = 0;
            if (lineCounter == 1) {
                if (bufferLine[0] == 'U' && bufferLine[1] == ' ') {
                    uni.size = setElementsCounter;
                    if (!fillArray(&uni.elements, bufferLine, setElementsCounter)) {
                        MEMORY_CLEAN
                        return 1;
                    }
                    // if (isBanned(&uni)) {
                    //     fprintf(stderr, "Your univerzum contains banned words!\n");
                    //     return 1;
                    // }
                    setElementsCounter = 0;
                } else {
                    fprintf(stderr, "You didn't set univerzum!\n");
                    return 1;
                }
            } else if (bufferLine[0] == 'S') {
                if (endOfLine(bufferLine[1]) || (bufferLine[1] == ' ' && endOfLine(bufferLine[2]))) {
                    sets[setCounter].elements = malloc(sizeof *(sets->elements));
                    sets[setCounter].elements[0] = malloc(sizeof **(sets->elements));
                    sets[setCounter].elements[0][0] = '\0';
                    sets[setCounter].size = 1;
                } else if (bufferLine[1] == ' ' && !endOfLine(bufferLine[2])) {
                    if (!fillArray(&sets[setCounter].elements, bufferLine, setElementsCounter)) {
                        return 1;
                    }
                    sets[setCounter].size = setElementsCounter;
                    setElementsCounter = 0;
                    for (int i = 0; i < sets[setCounter].size; i++) {
                        if (!isElementOfArray(&uni, &sets[setCounter].elements[i])) {
                            fprintf(stderr, "Element \"%s\" is not in the univerzum!\n", sets[setCounter].elements[i]);
                            return 1;
                        }
                    }
                }
                sets[setCounter].position = lineCounter;
                setCounter++;
                set* checkSets = realloc(sets, sizeof(set) * (setCounter + 1));
                if (checkSets == NULL) {
                    fprintf(stderr, "Cannot realloc sets!\n");
                    return 1;
                } else {
                    sets = checkSets;
                }
            } else if (bufferLine[0] == 'R' && bufferLine[1] == ' ') {
                if (!fillRelation(&relations[relationCounter].elements, bufferLine, relationElementsCounter)) {
                    return 1;
                }
                relations[relationCounter].position = lineCounter;
                relations[relationCounter].size = relationElementsCounter;
                relationElementsCounter = 0;
                relationCounter++;
                relation* checkRelations = realloc(relations, sizeof(relation) * (relationCounter + 1));
                if (checkRelations == NULL) {
                    fprintf(stderr, "Cannot realloc relations!\n");
                    return 1;
                } else {
                    relations = checkRelations;
                }
            } else {
                fprintf(stderr, "Line %d is not correct!\n", lineCounter);
                return 1;
            }
            printf("bufferLine: %s\n", bufferLine);
            clean(bufferLine);
            lineCounter++;
        }
    }

    for (int z = 0; z < uni.size; z++) {
        printf("univerzum[%d]: %s\n", z, uni.elements[z]);
        free(uni.elements[z]);
    }

    for (int i = 0; i < setCounter; i++) {
        for (int n = 0; n < sets[i].size; n++) {
            printf("sets[%d].elements[%d]: %s\n", i, n, sets[i].elements[n]);
            free(sets[i].elements[n]);
        }
        printf("sets[%d].position: %d\n", i, sets[i].position);
        free(sets[i].elements);
    }

    for (int i = 0; i < relationCounter; i++) {
        for (int f = 0; f < relations[i].size; f++) {
            for (int g = 0; g < 2; g++) {
                printf("relations[%d].elements[%d]: %s %s\n", i, f, relations[i].elements[f][g], relations[i].elements[f][g + 1]);
                free(relations[i].elements[f][g]);
                g++;
                free(relations[i].elements[f][g]);
            }
            free(relations[i].elements[f]);
        }
        printf("relations[%d].position: %d\n", i, relations[i].position);
        free(relations[i].elements);
    }

    free(uni.elements);
    free(bufferLine);
    free(sets);
    free(relations);
    fclose(file);

    return 0;
}
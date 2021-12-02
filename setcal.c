#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define SET_CLEAN {\
    for (int i = 0; i < setCounter; i++) {\
        if (sets[i].size == 0 && sets[i].elements[0][0] == '\0') {\
            free(sets[i].elements[0]);\
        } else {\
            cleanArray(&sets[i].elements, sets[i].size);\
        }\
        free(sets[i].elements);\
    }\
}\

#define RELATIONS_CLEAN {\
    for (int i = 0; i < relationCounter; i++) {\
        for (int f = 0; f < relations[i].size; f++) {\
            free(relations[i].elements[f][0]);\
            free(relations[i].elements[f][1]);\
            free(relations[i].elements[f]);\
        }\
        free(relations[i].elements);\
    }\
}\

#define SAR_CLEAN {\
    if (sets != NULL) {\
        free(sets);\
    }\
    if (relations != NULL) {\
        free(relations);\
    }\
    fclose(file);\
}\

#define  MEMORY_CLEAN {\
    cleanArray(&uni.elements, uni.size);\
    free(uni.elements);\
    free(bufferLine);\
    SET_CLEAN\
    RELATIONS_CLEAN\
    SAR_CLEAN\
}\

#define IS_SET_EMPTY {\
    if (elementsCounter == 0) {\
        if (endOfLine(source[1]) || (source[1] == ' ' && endOfLine(source[2]))) {\
            (*array) = malloc(sizeof *array);\
            (*array)[0] = malloc(1);\
            (*array)[0][0] = '\0';\
            return 1;\
        }\
    }\
}\

#define IS_RELATION_EMPTY {\
    if (relationsCounter == 0) {\
        if (endOfLine(source[1]) || (source[1] == ' ' && endOfLine(source[2]))) {\
            (*array) = malloc(sizeof *array);\
            (*array)[0] = malloc(2 * sizeof **array);\
            (*array)[0][0] = malloc(1);\
            (*array)[0][1] = malloc(1);\
            (*array)[0][0][0] = '\0';\
            (*array)[0][1][0] = '\0';\
            return 1;\
        }\
    }\
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
    char* bannedWords[6] = {"U", "C", "S", "R", "true", "false"};
    for (int i = 0; i < (int)strlen(source); i++) {
        for (int n = 0; n < 6; n++) {
            if(!strcmp(source, bannedWords[n])) {
                return true;
            }
        }
    }
    return false;
}

//Fills an array with elements from array source. Divider of elements is space
int fillArray(char*** array, char* source, int elementsCounter) {
    IS_SET_EMPTY

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
            if (source[start] == ' ' && (source[start - 1] == ' ' || endOfLine(source[start + 1]))) {
                fprintf(stderr, "You entered wrong line!\n");
                return 0;
            }
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
    IS_RELATION_EMPTY

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
                int repeat = 0;
                char* check[2] = {(*array)[positionInRelation][numberOfBinaryElement-1], (*array)[positionInRelation][numberOfBinaryElement]};
                for (int i = 0; i < positionInRelation; i++) {
                    repeat = 0;
                    for (int n = 0; n < 2; n++) {
                        if(!strcmp((*array)[i][n], check[n])) {
                            repeat++;
                        }
                        if (repeat == 2) {
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
        if (source[start] == ')') {
            if (source[start + 1] != ' ' || endOfLine(source[start + 1])) {
                fprintf(stderr, "Given relation is wrong!\n");
                return 0;
            } else if (source[start + 1] == ' ' && source[start + 2] != '(') {
                fprintf(stderr, "Given relation is wrong!\n");
                return 0;
            }
        }
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
    bool err_detector = false;

    while((bufferSymbol = fgetc(file)) != EOF) {
        char* checkBuffer = realloc(bufferLine, (symbolsCounter+2));
        if (checkBuffer == NULL) {
            fprintf(stderr, "Cannot realloc bufferLine!\n");
            err_detector = true;
            break;
        } else {
            bufferLine = checkBuffer;
        }
        if (!endOfLine(bufferSymbol)) {
            bufferLine[symbolsCounter] = (char)bufferSymbol;
            bufferLine[symbolsCounter + 1] = '\0';
            symbolsCounter++;
            if (bufferLine[0] == 'U' || bufferLine[0] == 'S') {
                if (bufferSymbol == ' ') {
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
                if (bufferLine[0] == 'U') {
                    if (setElementsCounter == 1 && bufferLine[1] == ' ' && bufferLine[2] == '\0') {
                        setElementsCounter--;
                    }
                    uni.size = setElementsCounter;
                    if (!fillArray(&uni.elements, bufferLine, setElementsCounter)) {
                        fprintf(stderr, "Cannot fill univerzum!\n");
                        err_detector = true;
                        break;
                    }
                    // if (isBanned(&uni)) {
                    //     fprintf(stderr, "Your univerzum contains banned words!\n");
                    //     return 1;
                    // }
                    setElementsCounter = 0;
                } else {
                    fprintf(stderr, "You didn't set univerzum!\n");
                    err_detector = true;
                    break;
                }
            } else if (bufferLine[0] == 'S') {
                if (setElementsCounter == 1 && bufferLine[1] == ' ' && bufferLine[2] == '\0') {
                        setElementsCounter--;
                }
                sets[setCounter].size = setElementsCounter;
                if (!fillArray(&sets[setCounter].elements, bufferLine, setElementsCounter)) {
                    fprintf(stderr, "Cannot fill set!\n");
                    err_detector = true;
                    setCounter++;
                    break;
                }
                setElementsCounter = 0;
                if (sets[setCounter].size != 0) {
                    for (int i = 0; i < sets[setCounter].size; i++) {
                        if (!isElementOfArray(&uni, &sets[setCounter].elements[i])) {
                            fprintf(stderr, "Element \"%s\" is not in the univerzum!\n", sets[setCounter].elements[i]);
                            err_detector = true;
                            break;
                        }
                    }
                }

                if (err_detector) {
                    break;
                }
                
                sets[setCounter].position = lineCounter;
                setCounter++;
                set* checkSets = realloc(sets, sizeof(set) * (setCounter + 1));
                if (checkSets == NULL) {
                    fprintf(stderr, "Cannot realloc sets!\n");
                    err_detector = true;
                    break;
                } else {
                    sets = checkSets;
                }
            } else if (bufferLine[0] == 'R') {
                relations[relationCounter].size = relationElementsCounter;
                if (!fillRelation(&relations[relationCounter].elements, bufferLine, relationElementsCounter)) {
                    fprintf(stderr, "Cannot fill relation!\n");
                    err_detector = true;
                    break;
                }
                relations[relationCounter].position = lineCounter;
                relationElementsCounter = 0;
                
                if (relations[relationCounter].size != 0) {
                    for (int i = 0; i < relations[relationCounter].size; i++) {
                        for (int n = 0; n < 2; n++) {
                            if (!isElementOfArray(&uni, &relations[relationCounter].elements[i][n])) {
                                fprintf(stderr, "Element \"%s\" is not in the univerzum!\n", relations[relationCounter].elements[i][n]);
                                relationCounter++;
                                err_detector = true;
                                break;
                            }
                        }
                        if (err_detector) {
                            break;
                        }
                    }
                }
                
                if (err_detector) {
                    break;
                }

                relationCounter++;
                relation* checkRelations = realloc(relations, sizeof(relation) * (relationCounter + 1));
                if (checkRelations == NULL) {
                    fprintf(stderr, "Cannot realloc relations!\n");
                    err_detector = true;
                    break;
                } else {
                    relations = checkRelations;
                }
            } else {
                fprintf(stderr, "Line %d is not correct!\n", lineCounter);
                err_detector = true;
                break;
            }
            printf("bufferLine: %s\n", bufferLine);
            clean(bufferLine);
            lineCounter++;
        }
    }

    if (err_detector) {
        MEMORY_CLEAN
        return 1;
    }

    for (int z = 0; z < uni.size; z++) {
        printf("univerzum[%d]: %s\n", z, uni.elements[z]);
    }

    for (int i = 0; i < setCounter; i++) {
        for (int n = 0; n < sets[i].size; n++) {
            printf("sets[%d].elements[%d]: %s\n", i, n, sets[i].elements[n]);
        }
        printf("sets[%d].position: %d\n", i, sets[i].position);
    }

    for (int i = 0; i < relationCounter; i++) {
        for (int f = 0; f < relations[i].size; f++) {
            printf("relations[%d].elements[%d]: %s %s\n", i, f, relations[i].elements[f][0], relations[i].elements[f][1]);
        }
        printf("relations[%d].position: %d\n", i, relations[i].position);
    }

    MEMORY_CLEAN

    return 0;
}
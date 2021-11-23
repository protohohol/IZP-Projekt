#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//Structure for sets
typedef struct {
    char** elements;
    int position;
} set;

//Structure for relations
typedef struct {
    char*** elements;
    int position;
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

//Fills an array with elements from array source. Divider of elements is space
int fillArray(char*** array, char* source) {
    (*array) = (char**)malloc(1);
    (*array)[0] = (char*)malloc(31);
    
    char element[31];
    int positionInElement = 0;
    int positionInArray = 0;
    int start = 2;  //Position of start, because source[0] is always identificator, source[1] is ' '
    while (!endOfLine(source[start]) && positionInElement <= 30) {
        element[positionInElement] = source[start];
        element[positionInElement + 1] = '\0';
        positionInElement++;
        if (endOfLine(source[start + 1]) || source[start + 1] == ' ') {
            char** checkArray = (char**)realloc((*array), (31 * (positionInArray + 2)));
            if (!checkArray) {
                fprintf(stderr, "Cannot realloc given array\n");
                return 0;
            } else {
                (*array) = checkArray;
            }
            (*array)[positionInArray + 1] = (char*)malloc(31);
            (*array)[positionInArray + 1][0] = '\0';
            strcpy((*array)[positionInArray], element);
            positionInArray++;                         
            clean(element);
            positionInElement = 0;
            if (endOfLine(source[start + 1])) {
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

/*Fills an array with elements from array source. Final array must be an array of binary relations
(every element of final array is the sequence of each of the two elements in brackets from source array)*/
int fillRelation(char**** array, char* source) {
    (*array) = (char***)malloc(1);
    (*array)[0] = (char**)malloc(2);
    (*array)[0][0] = (char*)malloc(31);
    (*array)[0][1] = (char*)malloc(31);
    
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
            if (numberOfBinaryElement == 0) {
                char*** checkArray = (char***)realloc((*array), (62 * (positionInRelation + 2)));
                if (!checkArray) {
                    fprintf(stderr, "Cannot realloc given array!\n");
                    return 0;
                } else {
                    (*array) = checkArray;
                    (*array)[positionInRelation + 1] = (char**)malloc(2);
                    (*array)[positionInRelation + 1][numberOfBinaryElement] = (char*)malloc(31);
                    (*array)[positionInRelation + 1][numberOfBinaryElement + 1] = (char*)malloc(31);
                    (*array)[positionInRelation + 1][0][0] = '\0';
                }
            }
            strcpy((*array)[positionInRelation][numberOfBinaryElement], element);
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

// bool isElementOfArray(char*** comparatorArray, char** comparable) {
//     printf("Fuck\n");
//     for (int i = 0; *comparatorArray[i][0] != '\0'; i++) {
//         printf("Fuck\n");
//         if(!strcmp(*comparatorArray[i], *comparable)) {
//             return true;
//         }
//     }
//     return false;
// }

int main(int argc, char **argv) {
    (void) argc;

    FILE *file;
    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Cannot open file!\n");
        return 1;
    }
    int bufferSymbol;
    char* bufferLine = (char*)malloc(1);
    int symbolsCounter = 0;
    int lineCounter = 1;
    char** univerzum;
    set* sets = (set*)malloc(sizeof(set));
    int setCounter = 0;
    relation* relations = (relation*)malloc(sizeof(relation));
    int relationCounter = 0;

    while((bufferSymbol = fgetc(file)) != EOF) {
        char* checkBuffer = (char*)realloc(bufferLine, (symbolsCounter+2));
        if (!checkBuffer) {
            fprintf(stderr, "Cannot realloc bufferLine!\n");
            return 1;
        } else {
            bufferLine = checkBuffer;
        }
        if (!endOfLine(bufferSymbol)) {
            bufferLine[symbolsCounter] = (char)bufferSymbol;
            bufferLine[symbolsCounter + 1] = '\0';
            symbolsCounter++;
        } else {
            symbolsCounter = 0;
            if (lineCounter == 1) {
                if (bufferLine[0] == 'U' && bufferLine[1] == ' ') {
                    if (!fillArray(&univerzum, bufferLine)) {
                        return 1;
                    }
                } else {
                    fprintf(stderr, "You didn't set univerzum!\n");
                    return 1;
                }
            } else if (bufferLine[0] == 'S' && bufferLine[1] == ' ') {
                if (!fillArray(&sets[setCounter].elements, bufferLine)) {
                    return 1;
                }
                // for (int i = 0; sets[setCounter].elements[i][0] != '\0'; i++) {
                //     if (!isElementOfArray(&univerzum, &sets[setCounter].elements[i])) {
                //         fprintf(stderr, "Element \"%s\" is not set in univerzum!\n", sets[setCounter].elements[i]);
                //         return 1;
                //     }
                // }
                sets[setCounter].position = lineCounter;
                setCounter++;
                set* checkSets = (set*)realloc(sets, sizeof(set) * (setCounter + 1));
                if (!checkSets) {
                    fprintf(stderr, "Cannot realloc sets!\n");
                    return 1;
                } else {
                    sets = checkSets;
                }
            } else if (bufferLine[0] == 'R' && bufferLine[1] == ' ') {
                if (!fillRelation(&relations[relationCounter].elements, bufferLine)) {
                    return 1;
                }
                relations[relationCounter].position = lineCounter;
                relationCounter++;
                relation* checkRelations = (relation*)realloc(relations, sizeof(relation) * (relationCounter + 1));
                if (!checkRelations) {
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

    for (int z = 0; univerzum[z][0] != '\0'; z++) {
        printf("univerzum[%d]: %s\n", z, univerzum[z]);
        free(univerzum[z]);
    }

    for (int i = 0; i < setCounter; i++) {
        for (int n = 0; sets[i].elements[n][0] != '\0'; n++) {
            printf("sets[%d].elements[%d]: %s\n", i, n, sets[i].elements[n]);
            free(sets[i].elements[n]);
        }
        printf("sets[%d].position: %d\n", i, sets[i].position);
        free(sets[i].elements);
    }

    for (int i = 0; i < relationCounter; i++) {
        for (int f = 0; relations[i].elements[f][0][0] != '\0'; f++) {
            for (int g = 0; g < 2; g++) {
                printf("relations[%d].elements[%d]: %s %s\n", i, f, relations[i].elements[f][g], relations[i].elements[f][g+1]);
                free(relations[i].elements[f][g]);
                g++;
                free(relations[i].elements[f][g]);
            }
            free(relations[i].elements[f]);
        }
        printf("relations[%d].position: %d\n", i, relations[i].position);
        free(relations[i].elements);
    }

    free(univerzum);
    free(relations);
    free(sets);
    free(bufferLine);
    fclose(file);

    return 0;
}
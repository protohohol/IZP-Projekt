#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//
// bool IsEmpy(char A) {}
// int Card(char A) {}
// char Complement(char A) {}
// char Union(char A, char B) {}
// char Intersect(char A, char B) {}
// char Minus(char A, char B) {}
// bool Subseteq(char A, char B) {}
// bool Subset(char A, char B) {}
// bool Equals(char A, char B) {}
// //
// bool Reflexive(char R) {}
// bool Symmetric(char R) {}
// bool Antisymmetric(char R) {}
// bool Transitive(char R) {}
// bool Function(char R) {}
// bool Domain(char R) {}
// bool Codomain(char R) {}
// bool Injective(char R) {}
// bool Surjective(char R) {}
// bool Bijective(char R) {}
//
void clean(char *c) {
    int count = 0;

    while(c[count] != '\0') {
        c[count] = '\0';
        count++;
    }
}

int fill_array(char*** array, char* source) {
    (*array) = (char**)malloc(1);
    (*array)[0] = (char*)malloc(31);
    
    char element[31];
    int g = 0;
    int f = 0;
    int start = 2;
    while (source[start] != '\n' && g <= 30) {
        element[g] = source[start];
        element[g+1] = '\0';
        g++;
        if (source[start+1] == '\n' || source[start+1] == '\0' || source[start+1] == ' ') {
            char** check = (char**)realloc((*array), (31 * (f + 2)));
            if (!check) {
                fprintf(stderr, "Smt is wrong ;(\n");
                return 0;
            } else {
                (*array) = check;
            }
            (*array)[f + 1] = (char*)malloc(31);
            strcpy((*array)[f], element);
            f++;                         
            clean(element);
            g = 0;
            if (source[start+1] == '\n' || source[start+1] == '\0') {
                break;
            }
        }
        start++;
    }
    if (g > 30) {
        fprintf(stderr, "Max length of argument is 30!\n");
        return 0;
    }
    return 1;
}

typedef struct {
    char** elements;
    int position;
} set;

typedef struct {
    char*** elements;
    int position;
} relation;

int fill_relation(char**** array, char* source) {
    (*array) = (char***)malloc(1);
    (*array)[0] = (char**)malloc(2);
    (*array)[0][0] = (char*)malloc(31);
    (*array)[0][1] = (char*)malloc(31);
    
    char element[31];
    int g = 0;
    int f = 0;
    int r = -1;
    int start = 2;
    int control = 1;
    while (source[start] != '\n' && g <= 30) {
        control = 1;
        if (source[start] == '(') {
            r++;
            f = 0;
            control = 0;
        } else {
            if (source[start] != '(' && source[start] != ')' && source[start] != ' ') {
                element[g] = source[start];
                element[g+1] = '\0';
                g++;
            }
        }
        if (f > 2) {
            fprintf(stderr, "Max amount of elements in relation is 2!");
            return 0;
        }
        if ((source[start+1] == '\n' || source[start+1] == '\0' || (source[start+1] == ' ' &&  source[start+2] != '(')|| source[start+1] == ')') && control) {
            if (f == 0) {
                char*** check = (char***)realloc((*array), (62 * (r + 2)));
                if (!check) {
                    fprintf(stderr, "Smt is wrong ;(\n");
                    return 0;
                } else {
                    (*array) = check;
                    (*array)[r + 1] = (char**)malloc(2);
                    (*array)[r + 1][f] = (char*)malloc(31);
                    (*array)[r + 1][f + 1] = (char*)malloc(31);
                }
            }
            strcpy((*array)[r][f], element);
            f++;                         
            clean(element);
            g = 0;
            if (source[start+1] == ')' && (source[start+2] == '\n' || source[start+2] == '\0')) {
                break;
            }
        }
        start++;
    }
    if (g > 30) {
        fprintf(stderr, "Max length of argument is 30!\n");
        return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    (void) argc;

    FILE *f = fopen(argv[1], "r");
    int buf;
    char* buffer = (char*)malloc(1);;
    int c = 0;
    int count = 0;
    char** uni;
    int am = 1;
    set* s1 = (set*)malloc(sizeof(set));
    int setc = 0;
    relation* r1 = (relation*)malloc(sizeof(relation));
    int relc = 0;

    while((buf = fgetc(f)) != EOF) {
        char* check = (char*)realloc(buffer, (c+2));
        if (!check) {
            fprintf(stderr, "Smt is wrong ;(\n");
            return 1;
        } else {
            buffer = check;
        }
        if (buf != '\n' && buf != '\0') {
            buffer[count] = (char)buf;
            buffer[count + 1] = '\0';
            count++;
        } else {
            count = 0;
            if (am == 1) {
                if (buffer[0] == 'U') {
                    if (!fill_array(&uni, buffer)) {
                        return 1;
                    }
                }
            }
            if (buffer[0] == 'S') {
                if (!fill_array(&s1[setc].elements, buffer)) {
                    return 1;
                }
                s1[setc].position = am;
                setc++;
                set* s2 = (set*)realloc(s1, sizeof(set) * (setc + 1));
                if (!s2) {
                    fprintf(stderr, "Smt is wrong ;(\n");
                    return 1;
                } else {
                    s1 = s2;
                }
            }
            if (buffer[0] == 'R') {
                if (!fill_relation(&r1[relc].elements, buffer)) {
                    return 1;
                }
                r1[relc].position = am;
                relc++;
                relation* r2 = (relation*)realloc(r1, sizeof(relation) * (relc + 1));
                if (!r2) {
                    fprintf(stderr, "Smt is wrong ;(\n");
                    return 1;
                } else {
                    r1 = r2;
                }
            }
            printf("buffer: %s\n", buffer);
            clean(buffer);
            am++;
        }
        c++;
    }

    for (int z = 0; uni[z][0] != '\0'; z++) {
        printf("uni[%d]: %s\n", z, uni[z]);
        free(uni[z]);
    }

    for (int i = 0; i < setc; i++) {
        for (int n = 0; s1[i].elements[n][0] != '\0'; n++) {
            printf("s1[%d].elements[%d]: %s\n", i, n, s1[i].elements[n]);
            free(s1[i].elements[n]);
        }
        printf("s1[%d].position: %d\n", i, s1[i].position);
        free(s1[i].elements);
    }

    for (int i = 0; i < relc; i++) {
        for (int f = 0; r1[i].elements[f][0][0] != '\0'; f++) {
            for (int g = 0; g < 2; g++) {
                printf("r1[%d].elements[%d]: %s %s\n", i, f, r1[i].elements[f][g], r1[i].elements[f][g+1]);
                free(r1[i].elements[f][g]);
                g++;
                free(r1[i].elements[f][g]);
            }
            free(r1[i].elements[f]);
        }
        printf("r1[%d].position: %d\n", i, r1[i].position);
        free(r1[i].elements);
    }

    free(uni);
    free(r1);
    free(s1);
    free(buffer);
    fclose(f);

    return 0;
}
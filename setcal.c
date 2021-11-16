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

int fill_array(char*** array, char* source, int start) {
    char element[31];
    int g = 0;
    int f = 0;
    while (source[start] != '\n' && g <= 30) {
        element[g] = source[start];
        element[g+1] = '\0';
        g++;
        if (source[start+1] == '\n' || source[start+1] == '\0' || source[start+1] == ' ') {
            char** check = (char**)realloc((*array), (31 * (f+2)));
            if (!check) {
                fprintf(stderr, "Smt is wrong ;(\n");
                return 0;
            } else {
                (*array) = check;
            }
            (*array)[f+1] = (char*)malloc(31);
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
    char* elements;
    int position;
} relation;

int main(int argc, char **argv) {
    (void) argc;

    FILE *f = fopen(argv[1], "r");
    int buf;
    char* buffer = (char*)malloc(1);;
    int c = 0;
    int count = 0;
    char** uni = (char**)malloc(1);
    uni[0] = (char*)malloc(31);
    int am = 1;
    set s1 = {.elements = (char**)malloc(1), .position = 0};
    s1.elements[0] = (char*)malloc(31);

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
                    if (!fill_array(&uni, buffer, 2)) {
                        return 1;
                    }
                }
            }
            if (buffer[0] == 'S') {
                    if (!fill_array(&s1.elements, buffer, 2)) {
                        return 1;
                    }
                    s1.position = am;
            
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

    for (int z = 0; s1.elements[z][0] != '\0'; z++) {
        printf("set.elements[%d]: %s\n", z, s1.elements[z]);
        free(s1.elements[z]);
    }

    free(uni);
    free(s1.elements);
    free(buffer);
    fclose(f);

    return 0;
}
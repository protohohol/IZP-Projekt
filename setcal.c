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
    char* buffer = (char*)malloc(sizeof(char) * 1);;
    int c = 0;
    int count = 0;
    char** uni = (char**)malloc(sizeof(char*) * 1);
    uni[0] = (char*)malloc(31 * sizeof(char));
    int am = 1;

    while((buf = fgetc(f)) != EOF) {
        buffer = (char*)realloc(buffer, sizeof(char) * (c+1));
        if (buf != '\n' && buf != '\0') {
            buffer[count] = (char)buf;
            buffer[count + 1] = '\0';
            count++;
        } else {
            count = 0;
            if (am == 1) {
                if (buffer[0] == 'U') {
                    int i = 2;
                    char element[31];
                    int g = 0;
                    int f = 0;
                    while (buffer[i] != '\n') {
                        element[g] = buffer[i];
                        element[g+1] = '\0';
                        g++;
                        if (buffer[i+1] == '\n' || buffer[i+1] == '\0' || buffer[i+1] == ' ') {
                            uni = (char**)realloc(uni, (sizeof(char*) * (f+2)));
                            uni[f+1] = (char*)malloc(31 * sizeof(char));
                            strcpy(uni[f], element);
                            f++;                         
                            clean(element);
                            g = 0;
                            if (buffer[i+1] == '\n' || buffer[i+1] == '\0') {
                                break;
                            }
                        }
                        i++;
                    }
                }
            }
            printf("buffer: %s\n", buffer);
            clean(buffer);
            am++;
        }
        c++;
    }

    for (int z = 0; uni[z][0] != '\0'; z++) {
        printf("uni.elements[%d]: %s\n", z, uni[z]);
        free(uni[z]);
    }
    
    free(uni);
    free(buffer);
    fclose(f);

    return 0;
}
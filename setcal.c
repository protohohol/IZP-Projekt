#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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

int main(int argc, char **argv) {
    (void) argc;

    FILE *f = fopen(argv[1], "r");
    int buf;
    char* buffer = (char*)malloc(sizeof(char) * 1);;
    int c = 0;
    int count = 0;

    while((buf = fgetc(f)) != EOF) {
        //printf("buf: %c\n", buf);
        buffer = (char*)realloc(buffer, sizeof(char) * c);
        if (buf != '\n' && buf != '\0') {
            //printf("hi!\n");
            buffer[count] = (char)buf;
            buffer[count + 1] = '\0';
            count++;
            //buffer[c+1] = '\0';
            //printf("hi!1\n");
        } else {
            count = 0;
            printf("buffer: %s\n", buffer);
            clean(buffer);
        }
        c++;
        //printf("buf: %c\n", buf);
    }

    free(buffer); 
    fclose(f);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

void processFile(FILE* in, FILE* out) {
    char ch, prevCh;

    while ((ch = fgetc(in)) != EOF) {
        if (ch == ' ' || ch == '\t') {
            if (prevCh != ' ') {
                fputc(' ', out);
            }
        } else {
            fputc(ch, out);
        }
        prevCh = ch;
    }
}

int main() {
    FILE* in, *out;

    in = fopen("q1in.c", "r");
    if (in == NULL) {
        printf("Cannot open file\n");
        exit(1);
    }

    out = fopen("q1out.c", "w");

    processFile(in, out);
}

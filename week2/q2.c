#include <stdio.h>
#include <stdlib.h>

void discardPreprocessorDirectives(FILE* in, FILE* out) {
    char line[1000];

    while (fgets(line, sizeof(line), in)) {
        if (line[0] != '#') {
            fputs(line, out);
        }
    }
}

int main() {
    FILE* in, *out;

    in = fopen("q2in.c", "r");
    if (in == NULL) {
        perror("Cannot open file\n");
        exit(1);
    }

    out = fopen("q2out.c", "w");

    discardPreprocessorDirectives(in, out);
}
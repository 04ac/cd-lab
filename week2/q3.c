#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_KEYWORDS 32

char *keywords[NUM_KEYWORDS] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int isKeyword(char *word) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(keywords[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

void toUpperWord(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = toupper(word[i]);
    }
}

void processFile(FILE* in) {
    char c;
    char word[500];
    int i = 0;

    while ((c = fgetc(in)) != EOF) {
        if (isalpha(c) || c == '_') { // Character in a word
            word[i++] = c;
        } else {
            if (i > 0) {
                word[i] = '\0';

                if (isKeyword(word)) {
                    toUpperWord(word); // convert word to uppercase
                }

                printf("%s", word);
                i = 0; // Reset word buffer
            }
            printf("%c", c); // print non word chars as is
        }
    }


    // For the case where the program ends with a word
    // A C program can end with a word if there is a comment on the last line
    if (i > 0) {
        word[i] = '\0';

        if (isKeyword(word)) {
            toUpperWord(word); // convert word to uppercase
        }

        printf("%s", word);
        i = 0; // Reset word buffer
    }
}

int main() {
    FILE* in, *out;

    in = fopen("q3in.c", "r");
    if (in == NULL) {
        printf("Cannot open file\n");
        exit(1);
    }

    processFile(in);
}
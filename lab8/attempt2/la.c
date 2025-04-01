#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef struct Token {
    char lexeme[50];
    char type[50];
    int row, col;
} Token;

char* ops = "+-*/\%!=<>|&^";

int isOperator(char c) {
    return strchr(ops, c) != NULL;
}

char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", 
    "do", "double", "else", "enum", "extern", "float", "for", "goto", 
    "if", "int", "long", "register", "return", "short", "signed", 
    "sizeof", "static", "struct", "switch", "typedef", "union", 
    "unsigned", "void", "volatile", "while"
};

int isKeyword(char* s) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
        if (strcmp(s, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char* splchars = ";:?.,[]{}()";

int isSplChar(char c) {
    return strchr(splchars, c) != NULL;
}

int row = 1, col = 1;
char c, d;

Token* getNextToken(FILE* f) {
    Token* t = (Token*) malloc(sizeof(Token));

    while ((c = fgetc(f)) != EOF) {
        t->row = row;
        t->col = col;

        if (c == '\n') {
            row++;
            col = 1;
            continue;
        }

        if (isspace(c)) {
            col++;
            continue;
        }

        if (c == '#') {
            while ((c = fgetc(f)) != EOF && c != '\n') {
                col++;
            }

            if (c == '\n') {
                row++;
                col = 1;
            }
            continue;
        }

        if (c == '/') {
            d = fgetc(f);
            col++;

            if (d == '/') {
                while ((c = fgetc(f)) != EOF && c != '\n') {
                    col++;
                }

                if (c == '\n') {
                    row++;
                    col = 1;
                }
                continue;
            } else if (d == '*') {
                while (c != EOF) {
                    while ((c = fgetc(f)) != '*') {
                        col++;

                        if (c == '\n') {
                            col = 1;
                            row++;
                        }
                    }

                    if ((d = fgetc(f)) != EOF && d == '/') {
                        col++;
                        break;
                    } else if (d == '\n') {
                        row++;
                        col = 1;
                    } else if (d == EOF) {
                        break;
                    }
                }
            } else {
                fseek(f, -1, SEEK_CUR);
                t->lexeme[0] = c;
                t->lexeme[1] = '\0';
                strcpy(t->type, "Operator");
                return t;
            }
        }

        if (isOperator(c)) {
            d = fgetc(f);

            if (isOperator(d)) {
                col += 2;
                t->lexeme[0] = c;
                t->lexeme[1] = d;
                t->lexeme[2] = '\0';
            } else {
                col++;
                fseek(f, -1, SEEK_CUR);
                t->lexeme[0] = c;
                t->lexeme[1] = '\0';
            }
            strcpy(t->type, "Operator");
            return t;
        }

        if (isdigit(c)) {
            int i = 0;
            t->lexeme[i++] = c;
            while ((c = fgetc(f)) != EOF && isdigit(c)) {
                col++;
                t->lexeme[i++] = c;
            }

            fseek(f, -1, SEEK_CUR);
            t->lexeme[i++] = '\0';
            strcpy(t->type, "Number");
            return t;
        }

        if (isalpha(c) || c == '_') {
            int i = 0;
            t->lexeme[i++] = c;

            while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) {
                t->lexeme[i++] = c;
                col++;
            }

            fseek(f, -1, SEEK_CUR);
            t->lexeme[i++] = '\0';

            if (isKeyword(t->lexeme)) {
                strcpy(t->type, "Keyword");
            } else {
                strcpy(t->type, "Identifier");
            }
            return t;
        }

        if (c == '"') {
            int i = 0;
            t->lexeme[i++] = c;
            while ((c = fgetc(f)) != EOF && c != '"') {
                col++;
                t->lexeme[i++] = c;
            }

            t->lexeme[i++] = c;
            col++;
            strcpy(t->type, "String Literal");
            return t;
        }

        if (c == '\'') {
            int i = 0; 
            t->lexeme[i++] = c;

            while ((c = fgetc(f)) != EOF && c != '\'') {
                col++;
                t->lexeme[i++] = c;
            }

            t->lexeme[i++] = c;
            col++;
            strcpy(t->type, "Character Literal");
            return t;
        }

        if (isSplChar(c)) {
            t->lexeme[0] = c;
            t->lexeme[1] = '\0';
            col++;
            strcpy(t->type, "Special Character");
            return t;
        }
    }

    strcpy(t->type, "eof");
    strcpy(t->lexeme, "eof");
    return t;
}

int k = 0;
void printToken(Token* t) {
    printf("%d, < %s, %s, %d, %d >\n", k++, t->lexeme, t->type, t->row, t->col);
}

// int main() {
//     FILE* f = fopen("../inp.c", "r");
//     Token* t;
//     while (strcmp((t = getNextToken(f))->type, "eof") != 0) {
//         printToken(t);
//     }
// }
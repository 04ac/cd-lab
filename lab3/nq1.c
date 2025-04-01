#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Token {
    char lexeme[50];
    char type[50];
    int row, col;
} Token;

char* operators = "+-*/=&!|\%^<>";

int isOperator(char c) {
    return strchr(operators, c) != NULL;
}

char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", 
    "do", "double", "else", "enum", "extern", "float", "for", "goto", 
    "if", "int", "long", "register", "return", "short", "signed", 
    "sizeof", "static", "struct", "switch", "typedef", "union", 
    "unsigned", "void", "volatile", "while"
};

int isKeyword(char* k) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
        if (strcmp(k, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char* spls = "(){}[];:.,?";

int isspls(char c) {
    return strchr(spls, c) != NULL;
}

int row = 1;
int col = 1;
char c;

Token* getNextToken(FILE* f) {
    Token* t = malloc(sizeof(Token));

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
                col = 1;
                row++;
            }
            continue;
        }

        if (c == '/') {
            char d = fgetc(f);
            col++;

            if (d == '/') {
                while ((c = fgetc(f)) != EOF && c != '\n') {
                    col++;
                }

                if (c == '\n') {
                    col = 1;
                    row++;
                }
            } else if (d == '*') {
                while ((c = fgetc(f)) != EOF && c != '*') {
                    col++;

                    if (c == '\n') {
                        row++;
                        col = 1;
                    } else if (c == '*') {
                        d = fgetc(f);
                        col++;

                        if (d == '/') break;
                    }
                }
            } else {
                fseek(f, -1, SEEK_CUR);
                col--;
                strcpy(t->lexeme, "/");
                strcpy(t->type, "Operator");
                return t;
            }
            continue;
        }

        if (isOperator(c)) {
            char d = fgetc(f);
            col++;

            if (isOperator(d)) {
                t->lexeme[0] = c;
                t->lexeme[1] = d;
                t->lexeme[2] = '\0';
            } else {
                fseek(f, -1, SEEK_CUR);
                col--;
                t->lexeme[0] = c;
                t->lexeme[1] = '\0';
            }

            strcpy(t->type, "Operator");

            return t;
        }

        if (c == '"') {
            int i = 0;
            t->lexeme[i++] = '"';
            while ((c = fgetc(f)) != EOF && c != '"') {
                col++;
                t->lexeme[i++] = c;
            }

            t->lexeme[i++] = c;
            t->lexeme[i++] = '\0';

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
            t->lexeme[i++] = '\0';
            strcpy(t->type, "Character Literal");
            return t;
        }

        if (isdigit(c)) {
            int i = 0;
            t->lexeme[i++] = c;
            while ((c = fgetc(f)) != EOF && isdigit(c)) {
                col++;
                t->lexeme[i++] = c;
            }
            t->lexeme[i++] = '\0';
            fseek(f, -1, SEEK_CUR);
            strcpy(t->type, "Number");
            return t;
        }

        if (isalpha(c) || c == '_') {
            int i = 0;
            t->lexeme[i++] = c;
            while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) {
                col++;
                t->lexeme[i++] = c;
            }

            fseek(f, -1, SEEK_CUR);
            col--;
            t->lexeme[i++] = '\0';

            if (isKeyword(t->lexeme)) {
                strcpy(t->type, "Keyword");
            } else {
                strcpy(t->type, "Identifier");
            }
            return t;
        }

        if (isspls(c)) {
            t->lexeme[0] = c;
            t->lexeme[1] = '\0';
            strcpy(t->type, "Special Symbol");
            return t;
        }
    }

    strcpy(t->lexeme, "eof");
    strcpy(t->type, "eof");
    return t;
}

int main() {
    FILE* f = fopen("inp.c", "r");
    int k = 0;
    Token* t;

    while (strcmp((t = getNextToken(f))->type, "eof") != 0) {
        printf("%d, < %s, %s, %d, %d >\n", k++, t->lexeme, t->type, t->row, t->col);
    }
}
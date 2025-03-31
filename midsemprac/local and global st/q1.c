#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Token {
    char lexeme[50];
    char type[50];
    int row;
    int col;
    struct Token* next;
} Token;

// Two symbol tables: global and local (each with a dummy head)
Token *globalSymbolTable, *globalTail;
Token *localSymbolTable, *localTail;

int row = 1, col = 1;
int currentScope = 0;  // 0 means global, >0 means inside a function

char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", 
    "do", "double", "else", "enum", "extern", "float", "for", "goto", 
    "if", "int", "long", "register", "return", "short", "signed", 
    "sizeof", "static", "struct", "switch", "typedef", "union", 
    "unsigned", "void", "volatile", "while"
};

int isKeyword(char* buf) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
        if (strcmp(buf, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

char operators[] = "+-*/=!^&|%<>";
int isOperator(char c) {
    return strchr(operators, c) != NULL;
}

char specialSymbols[] = {'?', ';', ':', ',', '(', ')', '{', '}', '.', '[', ']', '\0'};
int isSpecialSymbol(char c) {
    return strchr(specialSymbols, c) != NULL;
}

void init() {
    // Initialize global symbol table
    globalSymbolTable = (Token*) malloc(sizeof(Token));
    globalSymbolTable->next = NULL;
    globalTail = globalSymbolTable;
    
    // Initialize local symbol table
    localSymbolTable = (Token*) malloc(sizeof(Token));
    localSymbolTable->next = NULL;
    localTail = localSymbolTable;
}

void addToGlobalSymbolTable(Token* t) {
    for (Token* ptr = globalSymbolTable->next; ptr; ptr = ptr->next) {
        if (strcmp(ptr->lexeme, t->lexeme) == 0)
            return;
    }
    globalTail->next = t;
    globalTail = globalTail->next;
}

void addToLocalSymbolTable(Token* t) {
    for (Token* ptr = localSymbolTable->next; ptr; ptr = ptr->next) {
        if (strcmp(ptr->lexeme, t->lexeme) == 0)
            return;
    }
    localTail->next = t;
    localTail = localTail->next;
}

void printGlobalSymbolTable() {
    int k = 1;
    for (Token* t = globalSymbolTable->next; t; t = t->next) {
        printf("%d. < %s, %d, %d, %s >\n", k++, t->lexeme, t->row, t->col, t->type);
    }
}

void printLocalSymbolTable() {
    int k = 1;
    for (Token* t = localSymbolTable->next; t; t = t->next) {
        printf("%d. < %s, %d, %d, %s >\n", k++, t->lexeme, t->row, t->col, t->type);
    }
}

Token* getNextToken(FILE* f) {
    Token* t = (Token*) malloc(sizeof(Token));
    t->next = NULL;

    char c;
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
        // Preprocessor directive: skip entire line
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
        // Comments
        if (c == '/') {
            col++;
            char d = fgetc(f);
            if (d == '/') {
                while ((c = fgetc(f)) != EOF && c != '\n')
                    ;
                if (c == '\n') {
                    row++;
                    col = 1;
                }
                continue;
            } else if (d == '*') {
                col++;
                while (1) {
                    while ((c = fgetc(f)) != EOF && c != '*') {
                        if (c == '\n') {
                            row++;
                            col = 1;
                        } else {
                            col++;
                        }
                    }
                    d = fgetc(f);
                    if (d == '/') {
                        col++;
                        break;
                    }
                }
                continue;
            } else {
                fseek(f, -1, SEEK_CUR);
            }
        }
        // Operators
        if (isOperator(c)) {
            char d = fgetc(f);
            if (isOperator(d)) {
                t->lexeme[0] = c;
                t->lexeme[1] = d;
                t->lexeme[2] = '\0';
                col += 2;
            } else {
                t->lexeme[0] = c;
                t->lexeme[1] = '\0';
                fseek(f, -1, SEEK_CUR);
                col++;
            }
            strcpy(t->type, "Operator");
            return t;
        }
        // Identifiers and keywords
        if (isalpha(c) || c == '_') {
            int i = 0;
            t->lexeme[i++] = c;
            col++;
            while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) {
                t->lexeme[i++] = c;
                col++;
            }
            t->lexeme[i] = '\0';
            fseek(f, -1, SEEK_CUR);
            if (isKeyword(t->lexeme)) {
                strcpy(t->type, "Keyword");
                return t;
            }
            // For non-keyword identifiers, decide based on current scope.
            if (currentScope == 0) {
                // Global scope: peek to see if the next non-space character is '('
                long pos = ftell(f);
                int nextChar;
                while ((nextChar = fgetc(f)) != EOF && isspace(nextChar))
                    ;
                if (nextChar == '(') {
                    strcpy(t->type, "Function Identifier");
                } else {
                    strcpy(t->type, "Variable Identifier");
                }
                fseek(f, pos, SEEK_SET);
                // Add a copy to the global symbol table
                Token* newToken = (Token*) malloc(sizeof(Token));
                strcpy(newToken->lexeme, t->lexeme);
                strcpy(newToken->type, t->type);
                newToken->row = t->row;
                newToken->col = t->col;
                newToken->next = NULL;
                addToGlobalSymbolTable(newToken);
            } else {
                // Inside a function: treat as a local identifier
                strcpy(t->type, "Local Identifier");
                Token* newToken = (Token*) malloc(sizeof(Token));
                strcpy(newToken->lexeme, t->lexeme);
                strcpy(newToken->type, t->type);
                newToken->row = t->row;
                newToken->col = t->col;
                newToken->next = NULL;
                addToLocalSymbolTable(newToken);
            }
            return t;
        }
        // String literal
        if (c == '"') {
            int i = 0;
            t->lexeme[i++] = c;
            col++;
            while ((c = fgetc(f)) != EOF && c != '"') {
                col++;
                t->lexeme[i++] = c;
            }
            t->lexeme[i++] = c;
            col++;
            t->lexeme[i] = '\0';
            strcpy(t->type, "String Literal");
            return t;
        }
        // Character literal
        if (c == '\'') {
            int i = 0;
            t->lexeme[i++] = c;
            col++;
            while ((c = fgetc(f)) != EOF && c != '\'') {
                t->lexeme[i++] = c;
                col++;
            }
            t->lexeme[i++] = c;
            col++;
            t->lexeme[i] = '\0';
            strcpy(t->type, "Character Literal");
            return t;
        }
        // Number literal
        if (isdigit(c)) {
            int i = 0;
            t->lexeme[i++] = c;
            col++;
            while ((c = fgetc(f)) != EOF && (isdigit(c) || c == '.')) {
                col++;
                t->lexeme[i++] = c;
            }
            t->lexeme[i] = '\0';
            fseek(f, -1, SEEK_CUR);
            strcpy(t->type, "Number");
            return t;
        }
        // Special symbols
        if (isSpecialSymbol(c)) {
            t->lexeme[0] = c;
            t->lexeme[1] = '\0';
            col++;
            strcpy(t->type, "Special Symbol");
            // Update scope: increase when encountering '{' and decrease when encountering '}'
            if (strcmp(t->lexeme, "{") == 0) {
                currentScope++;
            } else if (strcmp(t->lexeme, "}") == 0) {
                currentScope--;
            }
            return t;
        }
    }
    strcpy(t->lexeme, "End of file");
    strcpy(t->type, "End of file");
    return t;
}

int main() {
    init();

    FILE* f = fopen("inp.c", "r");
    if (f == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    Token* t;
    int k = 1;
    while (strcmp((t = getNextToken(f))->lexeme, "End of file") != 0) {
        printf("%d. < %s, %d, %d, %s >\n", k++, t->lexeme, t->row, t->col, t->type);
    }

    printf("\nGlobal Symbol Table --------------\n");
    printGlobalSymbolTable();

    printf("\nLocal Symbol Table --------------\n");
    printLocalSymbolTable();

    fclose(f);
    return 0;
}

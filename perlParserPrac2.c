#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100 // Limit for symbol table

// Structure for tokens
struct token {
    char lexeme[64];
    char type[20];
    int row, col;
};

// Symbol table entry
struct Symbol {
    char name[64];
    char type[20];
} symbolTable[MAX_SYMBOLS];

int symbolCount = 0; // Symbol table index

// Keywords, special symbols, and operators
char *keywords[] = {"int", "float", "char", "double", "return", "if", "else", "while", "for", "struct"};
char specialSymbols[] = {';', '(', ')', '{', '}', '[', ']', ',', '.'};
char operators[] = {'+', '-', '*', '/', '=', '<', '>'};

// Function to check if a word is a keyword
int isKeyword(char *word) {
    for (int i = 0; i < sizeof(keywords) / sizeof(char *); i++) {
        if (strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Function to check if a character is a special symbol
int isSpecialSymbol(char ch) {
    for (int i = 0; i < sizeof(specialSymbols); i++) {
        if (ch == specialSymbols[i]) return 1;
    }
    return 0;
}

// Function to check if a character is an operator
int isOperator(char ch) {
    for (int i = 0; i < sizeof(operators); i++) {
        if (ch == operators[i]) return 1;
    }
    return 0;
}

// Function to add an identifier to the symbol table
void addToSymbolTable(char *name, char *type) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return; // Already in the table
        }
    }
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolCount++;
}

// Function to get tokens from file
void getNextToken(FILE *file) {
    char ch;
    int row = 1, col = 1;

    while ((ch = fgetc(file)) != EOF) {
        struct token tkn = {.row = row, .col = col};

        if (isspace(ch)) {
            if (ch == '\n') {
                row++;
                col = 1;
            } else {
                col++;
            }
            continue;
        }

        if (isalpha(ch) || ch == '_') { // Identifier or Keyword
            int i = 0;
            tkn.lexeme[i++] = ch;
            while ((ch = fgetc(file)) != EOF && (isalnum(ch) || ch == '_')) {
                tkn.lexeme[i++] = ch;
            }
            tkn.lexeme[i] = '\0';

            if (isKeyword(tkn.lexeme)) {
                strcpy(tkn.type, "Keyword");
            } else {
                strcpy(tkn.type, "Identifier");
                addToSymbolTable(tkn.lexeme, "Variable"); // Store in symbol table
            }

            fseek(file, -1, SEEK_CUR);
        } else if (isdigit(ch)) { // Number
            int i = 0;
            tkn.lexeme[i++] = ch;
            while ((ch = fgetc(file)) != EOF && isdigit(ch)) {
                tkn.lexeme[i++] = ch;
            }
            tkn.lexeme[i] = '\0';
            strcpy(tkn.type, "Number");
            fseek(file, -1, SEEK_CUR);
        } else if (isOperator(ch)) { // Operators
            tkn.lexeme[0] = ch;
            tkn.lexeme[1] = '\0';
            strcpy(tkn.type, "Operator");
        } else if (isSpecialSymbol(ch)) { // Special Symbols
            tkn.lexeme[0] = ch;
            tkn.lexeme[1] = '\0';
            strcpy(tkn.type, "SpecialSymbol");
        } else if (ch == '/') { // Comments
            char next = fgetc(file);
            if (next == '/') { // Single-line comment
                while ((ch = fgetc(file)) != EOF && ch != '\n');
                row++;
                col = 1;
                continue;
            } else if (next == '*') { // Multi-line comment
                while ((ch = fgetc(file)) != EOF) {
                    if (ch == '*' && fgetc(file) == '/') {
                        break;
                    }
                    if (ch == '\n') row++;
                }
                continue;
            } else {
                fseek(file, -1, SEEK_CUR);
                tkn.lexeme[0] = '/';
                tkn.lexeme[1] = '\0';
                strcpy(tkn.type, "Operator");
            }
        } else { // Unknown Token
            tkn.lexeme[0] = ch;
            tkn.lexeme[1] = '\0';
            strcpy(tkn.type, "Unknown");
        }

        printf("<%s, %s, Row: %d, Col: %d>\n", tkn.lexeme, tkn.type, tkn.row, tkn.col);
        col++;
    }
}

// Function to print the symbol table
void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("-------------------------\n");
    printf("| %-10s | %-10s |\n", "Name", "Type");
    printf("-------------------------\n");

    for (int i = 0; i < symbolCount; i++) {
        printf("| %-10s | %-10s |\n", symbolTable[i].name, symbolTable[i].type);
    }

    printf("-------------------------\n");
}

int main() {
    FILE *file = fopen("input.c", "r");
    if (!file) {
        printf("File not found!\n");
        return 1;
    }

    getNextToken(file); // Get tokens from the file
    fclose(file);

    printSymbolTable(); // Print symbol table at the end
    return 0;
}

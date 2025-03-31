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

Token* symbolTable, *p;

int row = 1, col = 1;

char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", 
    "do", "double", "else", "enum", "extern", "float", "for", "goto", 
    "if", "int", "long", "register", "return", "short", "signed", 
    "sizeof", "static", "struct", "switch", "typedef", "union", 
    "unsigned", "void", "volatile", "while"
};

int isKeyword(char* buf) {
	for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
		if (strcmp(buf, keywords[i]) == 0) return 1;
	}
	return 0;
}

char operators[] = "+-*/=!^&|%<>";

int isOperator(char c) {
	return strchr(operators, c) != NULL;
}

char specialSymbols[] =  {'?', ';', ':', ',', '(', ')', '{', '}', '.', '[', ']', '\0'};

int isSpecialSymbol(char c) {
	return strchr(specialSymbols, c) != NULL;
}

void init() {
	symbolTable = (Token*) malloc(sizeof(Token));
	p = symbolTable;
}

void addToSymbolTable(Token* t) {
	for (Token* ptr = symbolTable->next; ptr; ptr = ptr->next) {
		if (strcmp(ptr->lexeme, t->lexeme) == 0) {
			return;
		}
	}

	p->next = t;
	p = p->next;
}

void printSymbolTable() {
	int k = 1;
	for (Token* t = symbolTable->next; t; t = t->next) {
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

		// preprocessor
		if (c == '#') {
			while ((c = fgetc(f)) != EOF && c != '\n') {
				col++;
			}

			if (c == '\n') {
				row++;
				col = 1;
			}
		}

		// comments
		if (c == '/') {
			col++;
			char d = fgetc(f);

			if (d == '/') {
				while ((c = fgetc(f)) != EOF && c !='\n');

				if (c == '\n') {
					row++;
					col = 1;
				}
				continue;
			} else if (d == '*') {
				col++;
				while (1) {
					while ((c = fgetc(f)) != EOF && c !='*') {
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

		// operators
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

		// identfiers and keywords
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
			strcpy(t->type, "Identifier");
			addToSymbolTable(t);
			return t;
		}

		// string literal
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

		// char literal
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

		// number
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

		// special symbol
		if (isSpecialSymbol(c)) {
			t->lexeme[0] = c;
			t->lexeme[1] = '\0';
			col++;
			strcpy(t->type, "Special Symbol");
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

	Token* t;
	int k = 1;
	while (strcmp((t = getNextToken(f))->lexeme, "End of file") != 0) {
		printf("%d. < %s, %d, %d, %s >\n", k++, t->lexeme, t->row, t->col, t->type);
	}

	printf("\nSymbol Table --------------\n");
	printSymbolTable();
}
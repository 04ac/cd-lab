#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char operators[] = "+-*/=<>!";

int isOperator(char c) {
	return strchr(operators, c) != NULL;
}

char specialSymbols[] = ";:.()[]{}";

int isSpecialSymbol(char c) {
	return strchr(specialSymbols, c) != NULL;
}

char* keywords[] = {"scalar", "foreach"};

int isKeyword(char* buf) {
	for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
		if (strcmp(keywords[i], buf) == 0) {
			return 1;
		}
	}
	return 0;
}

typedef struct Token {
	char lexeme[50];
	int row;
	int col;
	char type[50];
	struct Token* next;
} Token;

Token* symbolTable;
Token* p;

void init() {
	symbolTable = (Token*) malloc(sizeof(Token));
	p = symbolTable;
}

void addToSymbolTable(Token* t) {
	for (Token* ptr = symbolTable; ptr; ptr = ptr->next) {
		if (strcmp(ptr->lexeme, t->lexeme) == 0) {
			return; // already present in symbol table
		}
	}
	p->next = t;
	p = p->next;
}

void printSymbolTable() {
	int k = 1;

	printf("\nSymbol Table ---------------\n\n");

	Token* t = symbolTable->next;
	while (t) {
		printf("%d. < %s, %d, %d, %s >\n", k++, t->lexeme, t->row, t->col, t->type);
		t = t->next;
	}
}

int row = 1;
int col = 1;

Token* getNextToken(FILE* f) {
	char c;
	Token* t = (Token*) malloc(sizeof(Token));
	t->next = NULL;

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

		// @_
		if (c == '@') {
			int i = 0;
			char d = fgetc(f);

			if (d == '_') {
				t->lexeme[0] = '@';
				t->lexeme[1] = '_';
				t->lexeme[2] = '\0';

				col += 2;
				strcpy(t->type, "@_");
				return t;
			} else {
				fseek(f, -1, SEEK_CUR);
				t->lexeme[0] = '@';
				strcpy(t->type, "Special Symbol");
				col++;
				return t;
			}
		}

		// comments start with #
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

		// variables
		if (c == '$') {
			int i = 0;
			t->lexeme[i++] = c;
			col++;

			c = fgetc(f);

			if (isalpha(c) || c == '_') {
				t->lexeme[i++] = c;
				col++;
				while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) {
					col++;
					t->lexeme[i++] = c;
				}

				fseek(f, -1, SEEK_CUR);
				t->lexeme[i] = '\0';
				strcpy(t->type, "Variable");
				addToSymbolTable(t);
				return t;
			} else {
				t->lexeme[i] = '\0';
				fseek(f, -1, SEEK_CUR);
				strcpy(t->type, "Special Symbol");
				return t;
			}
		}

		// keywords
		if (isalpha(c) || c == '_') {
			int i = 0;
			t->lexeme[i++] = c;
			col++;
			while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) {
				col++;
				t->lexeme[i++] = c;
			}

			fseek(f, -1, SEEK_CUR);
			t->lexeme[i] = '\0';

			if (isKeyword(t->lexeme)) {
				strcpy(t->type, "Keyword");
			} else {
				strcpy(t->type, "Unknown");
			}
			return t;
		}

		// operators
		if (isOperator(c)) {
			char d = fgetc(f);

			if (isOperator(d)) {
				t->lexeme[0] = c;
				t->lexeme[1] = d;
				t->lexeme[2] = '\0';

				strcpy(t->type, "Operator");
				col += 2;
				return t;
			} else {
				fseek(f, -1, SEEK_CUR);

				t->lexeme[0] = c;
				t->lexeme[1] = '\0';

				strcpy(t->type, "Operator");
				col++;
				return t;
			}
		}

		if (isSpecialSymbol(c)) {
			col++;
			t->lexeme[0] = c;
			t->lexeme[1] = '\0';
			strcpy(t->type, "Special Symbol");
			return t;
		}

		// Numbers
		if (isdigit(c)) {
			int i = 0;
			t->lexeme[i++] = c;

			while ((c = fgetc(f)) != EOF && isdigit(c)) {
				col++;
				t->lexeme[i++] = c;
			}

			t->lexeme[i] = '\0';
			strcpy(t->type, "Number");
			return t;
		}
	}

	strcpy(t->lexeme, "End of File");
	strcpy(t->type, "End of File");
	return t;
}

int main() {
	FILE* f = fopen("inp.pl", "r");

	init();

	Token* t;
	int k = 1;
	while (strcmp((t = getNextToken(f))->lexeme, "End of File") != 0) {
		printf("%d. < %s, %d, %d, %s >\n", k++, t->lexeme, t->row, t->col, t->type);
	}

	printSymbolTable();
}
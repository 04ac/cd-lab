#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct Token
{
	char lexeme[100];
	int row;
	int col;
	char type[100];
	struct Token* next;
} Token;

Token beginning;
Token* symbolTable = &beginning;
Token* p = &beginning;

int row = 1, col = 1;

char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", 
    "do", "double", "else", "enum", "extern", "float", "for", "goto", 
    "if", "int", "long", "register", "return", "short", "signed", 
    "sizeof", "static", "struct", "switch", "typedef", "union", 
    "unsigned", "void", "volatile", "while"
};

int isKeyword(char* s) {
	for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
		if (strcmp(keywords[i], s) == 0) {
			return 1;
		}
	}
	return 0;
}

char operators[] = "+-/*=<>!&|";

int isOperator(char c) {
	return strchr(operators, c) != NULL;
}

char specialsymbols[] = {'?', ';', ':', ',', '(', ')', '{', '}', '.', '[', ']', '\0'};

int isSplSymbol(char c) {
	return strchr(specialsymbols, c) != NULL;
}

void addToSymbolTable(Token* t) {
	for (Token* ptr = symbolTable; ptr; ptr = ptr->next) {
		if (strcmp(ptr->lexeme, t->lexeme) == 0) {
			return; // already there in symbol table
		}
	}

	p->next = t;
	p = p->next;
}

void printSymbolTable() {
	Token* c = symbolTable->next;

	int k = 1;
	while (c) {
		printf("%d. lexeme: %s\ttype: %s\trow: %d\tcol: %d\n", k++, c->lexeme, c->type, c->row, c->col);
		c = c->next;
	}
}

Token* getNextToken(FILE* f) {
	Token* t = malloc(sizeof(Token));
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

		// headers
		if (c == '#') {
			while ((c = fgetc(f)) != EOF && c != '\n');

			if (c == '\n') {
				row++;
				col = 1;
			}
			continue;
		}

		// comments
		if (c == '/') {
            char d = fgetc(f);
            if (d == '/') {  
                // Skip until newline
                while ((c = fgetc(f)) != EOF && c != '\n');

                if (c == '\n') {
            		row++;
            		col = 1;
            	}
            	continue;
            }
            else if (d == '*') {  
                // Multi-line comment: Skip until */
                while ((c = fgetc(f)) != EOF) {
                	if (c == '\n') {
                		row++;
                		col = 1;
                	}

                    if (c == '*') {
                        d = fgetc(f);
                        if (d == '/') break;  // End of multi-line comment
                        else fseek(f, -1, SEEK_CUR);  // Put back if not '/'
                    }
                }
                continue;
            }
            else {
                fseek(f, -1, SEEK_CUR);  // Not a comment, put character back
            }
        }

		// special symbols
		if (isSplSymbol(c)) {
			t->lexeme[0] = c;
			t->lexeme[1] = '\0';
			
			col++;
			strcpy(t->type, "Special Symbol");
			return t;
		}

		// string literals
		if (c == '"') {
			int i = 0;
			t->lexeme[i++] = c;
			col++;

			while ((c = fgetc(f)) != EOF && c != '"') {
				t->lexeme[i++] = c;
				col++;
			}

			t->lexeme[i++] = c;
			t->lexeme[i] = '\0';
			col++;

			strcpy(t->type, "String Literal");

			return t;
		}

		// character literals
		if (c == '\'') {
			int i = 0;
			t->lexeme[i++] = c;
			col++;

			while ((c = fgetc(f)) != EOF && c != '\'') {
				t->lexeme[i++] = c;
				col++;
			}

			t->lexeme[i++] = c;
			t->lexeme[i] = '\0';
			col++;

			strcpy(t->type, "Character Literal");

			return t;
		}

		// numbers
		if (isdigit(c)) {
			int i = 0;
			t->lexeme[i++] = c;
			col++;

			while ((c = fgetc(f)) != EOF && isdigit(c)) {
				t->lexeme[i++] = c;
				col++;
			}

			t->lexeme[i] = '\0';
			fseek(f, -1, SEEK_CUR);
			strcpy(t->type, "Number");

			return t;
		}

		// identifiers & keywords
		if (isalpha(c) || c == '_') {
			int i = 0;
			t->lexeme[i++] = c;
			col++;

			while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_')) {
				t->lexeme[i++] = c;
				col++;
			}

			t->lexeme[i] = '\0';

			if (isKeyword(t->lexeme)) {
				strcpy(t->type, "Keyword");
			} else {
				strcpy(t->type, "Identifier");
				addToSymbolTable(t);
			}

			fseek(f, -1, SEEK_CUR);
			return t;
		}

		// operator
		if (isOperator(c)) {
			char d = fgetc(f);

			if (isOperator(d)) {
				t->lexeme[0] = c;
				t->lexeme[1] = d;
				t->lexeme[2] = '\0';

				col += 2;
			} else {
				fseek(f, -1, SEEK_CUR);

				t->lexeme[0] = c;
				t->lexeme[1] = '\0';

				col++;
			}

			strcpy(t->type, "Operator");
			return t;
		}
	}

	strcpy(t->lexeme, "End of File");
	strcpy(t->type, "End of File");
	t->row = row;
	t->col = col;

	return t;
}

int main() {
	FILE* f = fopen("inp.c", "r");

	Token* c;
	int k = 1;
	while (strcmp((c = getNextToken(f))->lexeme, "End of File") != 0) {
		printf("%d. lexeme: %s\ttype: %s\trow: %d\tcol: %d\n", k++, c->lexeme, c->type, c->row, c->col);
	}

	printf("\nSymbol Table--------------\n\n");
	printSymbolTable();
}
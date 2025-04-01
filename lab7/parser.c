#include "la.h"

void program();
void declarations();
void ass_stat();
int dtype();
void idlist();
void ass_stat_prime();
void idlist_prime();

Token* curr;
FILE* f;
int k = 0;

void match() {
    curr = getNextToken(f);
    if (strcmp(curr->type, "eof") == 0) {
        return;
    }

    printf("%d, < %s, %s, %d, %d >\n", k++, curr->lexeme, curr->type, curr->row, curr->col);
}

void valid() {
    printf("Success\n");
    exit(0);
}

void invalid(char* exp) {
    printf("Error at row %d and col %d: Expected %s\n", curr->row, curr->col, exp);
    exit(1);
}

void program() {
    match();
    if (strcmp(curr->lexeme, "main") == 0) {
        match();
        if (strcmp(curr->lexeme, "(") == 0) {
            match();
            if (strcmp(curr->lexeme, ")") == 0) {
                match();
                if (strcmp(curr->lexeme, "{") == 0) {
                    match();
                    declarations();
                    ass_stat();

                    if (strcmp(curr->lexeme, "}") == 0) {
                        match();
                    } else {
                        invalid("}");
                    }
                } else
                    invalid("{");
            } else
                invalid(")");
        } else
            invalid("(");
    } else
        invalid("main");
}

int dtype() {
    if (strcmp(curr->lexeme, "int") == 0 || strcmp(curr->lexeme, "char") == 0) {
        match();
        return 1;
    }
    return 0;
}

void declarations() {
    if (dtype()) {
        idlist();
        if (strcmp(curr->lexeme, ";") == 0) {
            match();
            declarations();
        } else {
            invalid(";");
        }
    }
}

void idlist() {
    if (strcmp(curr->type, "Identifier") == 0) {
        match();
        idlist_prime();
    } else {
        invalid("Identifier");
    }
}

void idlist_prime() {
    if (strcmp(curr->lexeme, ",") == 0) {
        match();
        idlist();
    }
}

void ass_stat() {
    if (strcmp(curr->type, "Identifier") == 0) {
        match();

        if (strcmp(curr->lexeme, "=") == 0) {
            match();
            ass_stat_prime();
        } else {
            invalid("=");
        }
    } else {
        invalid("Identifier");
    }
}

void ass_stat_prime() {
    if (strcmp(curr->type, "Identifier") == 0) {
        match();
        if (strcmp(curr->lexeme, ";") == 0) {
            match();
        } else {
            invalid(";");
        }
    } else if (strcmp(curr->type, "Number") == 0) {
        match();
        if (strcmp(curr->lexeme, ";") == 0) {
            match();
        } else {
            invalid(";");
        }
    } else {
        invalid("Identifier or Number");
    }
}

int main() {
    f = fopen("inp.c", "r");
    program();

    if (strcmp(curr->type, "eof") == 0) {
        valid();
    } else {
        invalid("eof");
    }
}
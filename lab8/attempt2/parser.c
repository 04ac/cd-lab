#include "la.c"

void program();
void declarations();
int dtype();
void idlist();
void idlist1();
void idlist2();
void statementlist();
int statement();
int assignstat();
int decisionstat();
void expn();
void simpleexpn();
void eprime();
int relop();
void term();
void seprime();
int addop();
void factor();
void tprime();
int mulop();
void dprime();


FILE* f;
Token* t;

void valid() {
    printf("Success!\n");
    exit(0);
}

void invalid(char* str) {
    printf("Error: expected %s at row %d and col %d\n", str, t->row, t->col);
    printToken(t);
    exit(1);
}

void match() {
    t = getNextToken(f);
    printToken(t);
}

void program() {
    match();
    if (strcmp(t->lexeme, "main") == 0) {
        match();
        if (strcmp(t->lexeme, "(") == 0) {
            match();
            if (strcmp(t->lexeme, ")") == 0) {
                match();
                if (strcmp(t->lexeme, "{") == 0) {
                    match();
                    declarations();
                    statementlist();
                    if (strcmp(t->lexeme, "}") == 0) {
                        match();
                    } else {
                        invalid("}");
                    }
                } else {
                    invalid("{");
                }
            } else {
                invalid(")");
            }
        } else {
            invalid("(");
        }
    } else {
        invalid("main");
    }
}

void declarations() {
    if (dtype()) {
        idlist();
        if (strcasecmp(t->lexeme, ";") == 0) {
            match();
            declarations();
        }
    }
}

int dtype() {
    if (strcmp(t->lexeme, "int") == 0 || strcmp(t->lexeme, "char") == 0) {
        match();
        return 1;
    } else {
        return 0;
    }
}

void idlist() {
    if (strcmp(t->type, "Identifier") == 0) {
        match();
        idlist1();
    } else {
        invalid("Identifier");
    }
}

void idlist1() {
    if (strcmp(t->lexeme, ",") == 0) {
        match();
        idlist();
    } else if (strcmp(t->lexeme, "[") == 0) {
        match();
        if (strcmp(t->type, "Number") == 0) {
            match();
            if (strcmp(t->lexeme, "]") == 0) {
                match();
                idlist2();
            } else {
                invalid("]");
            }
        } else {
            invalid("Number");
        }
    }
}

void idlist2() {
    if (strcmp(t->lexeme, ",") == 0) {
        match();
        idlist();
    }
}

void statementlist() {
    if (statement()) {
        statementlist();
    }
}

int statement() {
    if (assignstat()) {
        if (strcmp(t->lexeme, ";") == 0) {
            match();
        } else {
            invalid(";");
        }
        return 1;
    } else if (decisionstat()) {
        return 1;
    }
    return 0;
}

int assignstat() {
    if (strcmp(t->type, "Identifier") == 0) {
        match();
        if (strcmp(t->lexeme, "=") == 0) {
            match();
            expn();
        } else {
            invalid("=");
        }
        return 1;
    }
    return 0;
}

void expn() {
    simpleexpn();
    eprime();
}

void eprime() {
    if (relop()) {
        simpleexpn();
    }
}

void simpleexpn() {
    term();
    seprime();
}

void seprime() {
    if (addop()) {
        term();
        seprime();
    }
}

void term() {
    factor();
    tprime();
}

void tprime() {
    if (mulop()) {
        factor();
        tprime();
    }
}

void factor() {
    if (strcmp(t->type, "Identifier") == 0 || strcmp(t->type, "Number") == 0) {
        match();
    } else {
        invalid("Id | Num");
    }
}

int decisionstat() {
    if (strcmp(t->lexeme, "if") == 0) {
        match();
        if (strcmp(t->lexeme, "(") == 0) {
            match();
            expn();
            if (strcmp(t->lexeme, ")") == 0) {
                match();
                if (strcmp(t->lexeme, "{") == 0) {
                    match();
                    statementlist();
                    if (strcmp(t->lexeme, "}") == 0) {
                        match();
                        dprime();
                    } else invalid("}");
                } else invalid("{");
            } else invalid(")");
        } else invalid("(");
        return 1;
    }
    return 0;
}

void dprime() {
    if (strcmp(t->lexeme, "else") == 0) {
        match();
        if (strcmp(t->lexeme, "{") == 0) {
            match();
            statementlist();
            if (strcmp(t->lexeme, "}") == 0) {
                match();
            } else invalid("}");
        } else invalid("{");
    }
}

int relop() {
    if (strcmp(t->lexeme, "==") == 0
    || strcmp(t->lexeme, "!=") == 0
    || strcmp(t->lexeme, "<=") == 0
    || strcmp(t->lexeme, ">=") == 0
    || strcmp(t->lexeme, ">") == 0
    || strcmp(t->lexeme, "<") == 0) {
        match();
        return 1;
    }
    return 0;
}

int addop() {
    if (
        strcmp(t->lexeme, "+") == 0
        || strcmp(t->lexeme, "-") == 0
    ) {
        match();
        return 1;
    }
    return 0;
}

int mulop() {
    if (
        strcmp(t->lexeme, "*") == 0 ||
        strcmp(t->lexeme, "/") == 0 ||
        strcmp(t->lexeme, "%") == 0
    ) {
        match();
        return 1;
    }
    return 0;
}

int main() {
    f = fopen("../inp.c", "r");
    program();

    if (strcmp(t->lexeme, "eof") == 0) {
        valid();
    } else {
        invalid("eof");
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "la.h"

/* AFTER LEFT FACTORING IDENTIFIER_LIST
Program -> main(){ declarations statement-list } 
declarations -> data-type identifier-list; declarations | E
data-type -> int | char
identifier-list -> id identifier-list1
identifier-list1 -> E | , identifier-list | [number]identifier-list2
identifier-list2 -> , identifier-list | E
statement_list -> statement statement_list | E
statement -> assign-stat; | decision_stat
assign_stat -> id = expn
expn -> simple-expn eprime 
eprime -> relop simple-expn | E 
simple-expn -> term seprime 
seprime -> addop term seprime | E
term -> factor tprime
tprime -> mulop factor tprime | E
factor -> id | num
decision-stat -> if (expn) {statement_list} dprime 
dprime -> else {statement_list} | E
relop -> ==|!=|<=|>=|>|< 
addop -> +|- 
mulop -> *|/|%
*/

program();
declarations();
statementlist();
datatype();
idlist();
idlist1();
idlist2();
statement();
assignstat();
decisionstat();
expn();
simpleexpn();
eprime();
term();
seprime();
factor();
tprime();
relop();
addop();
mulop();
dprime();


FILE* f;
Token* t;
int k = 0;

void match() {
    t = getNextToken(f);
    if (strcmp(t->type, "eof") == 0) {
        return;
    }
    printf("%d, < %s, %s, %d, %d >\n", k++, t->lexeme, t->type, t->row, t->col);
}

void valid() {
    printf("Success!\n");
    exit(0);
}

void invalid(char* err) {
    printf("Error %s at row %d col %d\n", err, t->row, t->col);
    printf("%d, < %s, %s, %d, %d >\n", k++, t->lexeme, t->type, t->row, t->col);
    exit(1);
}

program() {
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

declarations() {
    if (datatype()) {
        idlist();

        if (strcmp(t->lexeme, ";") == 0) {
            match();
            declarations();
        } else {
            invalid(";");
        }
    }
}

datatype() {
    if (strcmp(t->lexeme, "int") == 0 || strcmp(t->lexeme, "char") == 0) {
        match();
        return 1;
    }
    return 0;
}

idlist() {
    if (strcmp(t->type, "Identifier") == 0) {
        match();
        idlist1();
    } else {
        invalid("Identifier");
    }
}

idlist1() {
    if (strcmp(t->lexeme, "[") == 0) {
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
    } else if (strcmp(t->lexeme, ",") == 0) {
        match();
        idlist();
    }
}

idlist2() {
    if (strcmp(t->lexeme, ",") == 0) {
        match();
        idlist();
    }
}

statementlist() {
    if (statement()) {
        statementlist();
    }
}

statement() {
    if (assignstat()) {
        if (strcmp(t->lexeme, ";") == 0) {
            match();
            return 1;
        } else {
            invalid(";");
        }
    } else if (decisionstat()) {
        // return 1;
    } else
        return 0;
}

assignstat() {
    if (strcmp(t->type, "Identifier") == 0) {
        match();
        if (strcasecmp(t->lexeme, "=") == 0) {
            match();
            expn();
            return 1;
        } else {
            invalid("=");
            // return 0;
        }
    } else {
        // invalid("Identifier");
        return 0;
    }
}

expn() {
    simpleexpn();
    eprime();
}

eprime() {
    if (relop()) {
        simpleexpn();
    }
}

simpleexpn() {
    term();
    seprime();
}

seprime() {
    if (addop()) {
        term();
        seprime();
        return 1;
    } else {
        // invalid("Addop");
        return 0;
    }
}

term() {
    factor();
    tprime();
}

tprime() {
    if (mulop()) {
        factor();
        tprime();
        return 1;
    }
    return 0;
}

factor() {
    if (strcmp(t->type, "Identifier") == 0
        || strcmp(t->type, "Number") == 0) {
        match();
        return 1;
    }
    invalid("Id | Number");
    return 0;
}

decisionstat() {
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
                    } else {
                        invalid("}");
                    }
                } else {
                    invalid("}");
                }
            } else {
                invalid(")");
            }
        } else {
            invalid("(");
        }
        return 1;
    } else {
        // invalid("if");
        return 0;
    }
}

dprime() {
    if (strcmp(t->lexeme, "else") == 0) {
        match();
        if (strcmp(t->lexeme, "{") == 0) {
            match();
            statementlist();
            if (strcmp(t->lexeme, "}") == 0) {
                match();
            } else {
                invalid("}");
            }
        } else {
            invalid("{");
        }
    }
}

relop() {
    if (strcmp(t->lexeme, "==") == 0 
        || strcmp(t->lexeme, "!=") == 0
        || strcmp(t->lexeme, "<=") == 0 
        || strcmp(t->lexeme, ">=") == 0 
        || strcmp(t->lexeme, ">") == 0 
        || strcmp(t->lexeme, "<") == 0) {
        match();
        return 1;
    }
    // invalid("==|!=|<=|>=|>|<");
    return 0;
}

addop() {
    if (strcmp(t->lexeme, "+") == 0 || strcmp(t->lexeme, "-") == 0) {
        match();
        return 1;
    }
    // invalid("+ | -");
    return 0;
}

mulop() {
    if (strcmp(t->lexeme, "*") == 0 || strcmp(t->lexeme, "/") == 0
    || strcmp(t->lexeme, "\%") == 0) {
        match();
        return 1;
    }
    // invalid("* | / | \%");
    return 0;
}

int main() {
    f = fopen("inp.c", "r");
    program();

    if (strcmp(t->lexeme, "eof") == 0) {
        valid();
    } else {
        invalid("eof");
    }
}
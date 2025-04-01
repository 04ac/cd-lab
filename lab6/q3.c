#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void valid() {
    printf("Success!\n");
    exit(0);
}

void invalid() {
    printf("Failure\n");
    exit(0);
}

int i = 0;
char str[50];

void S();
void A();
void Ap();
void B();

void S() {
    if (str[i] == 'a') {
        i++;
        A();
        if (str[i] == 'c') {
            i++;
            B();
            if (str[i] == 'e') {
                i++;
                return;
            } else {
                invalid();
            }
        } else {
            invalid();
        }
    } else {
        invalid();
    }
}

void A() {
    if (str[i] == 'b') {
        i++;
        Ap();
    } else {
        invalid();
    }
}

void Ap() {
    if (str[i] == 'b') {
        i++;
        Ap();
    }
}

void B() {
    if (str[i] == 'd') {
        i++;
        return;
    } else {
        invalid();
    }
}

int main() {
    printf("Enter string\n");
    scanf("%s", str);

    S();
    if (str[i] == '$') {
        valid();
    } else {
        invalid();
    }
}
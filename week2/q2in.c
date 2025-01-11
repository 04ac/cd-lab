#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int main() {
    int* a = malloc(1 * sizeof(int));

    free(a);
}
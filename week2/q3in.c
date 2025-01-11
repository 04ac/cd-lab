#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int main() {
    int* a = malloc(1 * sizeof(int));

    if (a == NULL) {
        printf("Error\n");
        return 1;
    }

    free(a);
    return 0;
}
// This program ends with a word.
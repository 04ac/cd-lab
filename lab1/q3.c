#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* f1 = fopen("file1.txt", "r");
	FILE* f2 = fopen("file2.txt", "r");

	FILE* f3 = fopen("q3out.txt", "w");

	char c1, c2;
	while (c1 != EOF || c2 != EOF) {
		if (c1 != EOF) {
			while ((c1 = fgetc(f1)) != EOF && c1 != '\n') {
				fputc(c1, f3);
			}
			fputc('\n', f3);
		}

		if (c2 != EOF) {
			while ((c2 = fgetc(f2)) != EOF && c2 != '\n') {
				fputc(c2, f3);
			}
			fputc('\n', f3);
		}
	}
}
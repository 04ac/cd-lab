#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* f = fopen("inp.c", "r");
	FILE* fd = fopen("out.c", "w");

	char c = fgetc(f);
	while (c != EOF) {
		if (c == '#') {
			while (c != EOF && c != '\n')
				c = fgetc(f);

			fputc('\n', fd);
			c = fgetc(f);
		} else {
			while (c != EOF) {
				fputc(c, fd);
				c = fgetc(f);
			}
		}
	}
}
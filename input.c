#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
	FILE* f = fopen("inp.c", "r");
	FILE* fd = fopen("out.c", "w");

	char c;
	c = fgetc(f);

	while (c != EOF) {
		if (c == '\t') {
			fputc(' ', fd);
		} else if (c == ' ') {
			fputc(' ', fd);
			char c2 = fgetc(f);

			if (c2 != ' ' || c2 != '\t') {
				fputc(c2, fd);
				break;
			}

			while (c2 != EOF && (c2 == ' ' || c2 == '\t')) {
				c2 = fgetc(f);
			}
			fputc(c2, fd);
		} else {
			fputc(c, fd);
		}
		c = fgetc(f);
	}
}


// muleieimfr     mfeiowemfvi wg wrmgiwremgfrwoigf reg     gfm
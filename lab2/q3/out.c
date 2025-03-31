#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

INT main() {
	FILE* f = fopen("inp.c", "r");
	FILE* fd = fopen("out.c", "w");

	CHAR c;
	c = fgetc(f);

	WHILE (c != EOF) {
		IF (c == '\t') {
			fputc(' ', fd);
		} ELSE IF (c == ' ') {
			fputc(' ', fd);
			CHAR c2 = fgetc(f);

			IF (c2 != ' ' || c2 != '\t') {
				fputc(c2, fd);
				BREAK;
			}

			WHILE (c2 != EOF && (c2 == ' ' || c2 == '\t')) {
				c2 = fgetc(f);
			}
			fputc(c2, fd);
		} ELSE {
			fputc(c, fd);
		}
		c = fgetc(f);
	}
}


muleieimfr     mfeiowemfvi wg wrmgiwremgfrwoigf reg     gfm
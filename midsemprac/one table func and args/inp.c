#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int a = 56;

int main() {
	FILE* f = fopen("inp.c", "r");
	FILE* fd = fopen("out.c", "w");

	/*bjdcbiernvire
	reviernviet
	*/ int b = 3;

	char c;
	// nikrnfo34nof34
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

	nfweunbfiw(3, "vdnibdr", 'c');
}


// muleieimfr     mfeiowemfvi wg wrmgiwremgfrwoigf reg     gfm


void nfweunbfiw(int kjjb, char* nifnvge, char erunbiuer) {

}


int g;
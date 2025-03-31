#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* f = fopen("sample_inp.c", "r");
	FILE* fd = fopen("sample_out.c", "w");

	char c;
	while ((c = fgetc(f)) != EOF) {
		if (c == '/') {
			char c2 = fgetc(f);

			if (c2 == '/') {
				while ((c = fgetc(f)) != EOF && c != '\n');

				if (c == '\n') fputc(c, fd);
			} else if (c2 == '*') {
				c = fgetc(f);

				while (c != EOF) {
					if (c == '*') {
						c = fgetc(f);

						if (c == EOF || c == '/') {
							break;
						}
					}
					c = fgetc(f);
				}
			} else {
				fputc(c, fd);
				c = c2;
			}
		} else {
			fputc(c, fd);
		}
	}
}
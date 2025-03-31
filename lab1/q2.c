#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* f = fopen("inp.txt", "r");
	FILE* fd = fopen("out.txt", "w");

	char c;
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);

	for (int i = sz - 1; i >= 0; i--) {
		fseek(f, i, SEEK_SET);
		c = fgetc(f);
		fputc(c, fd);
	}
}
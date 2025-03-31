#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void capAll(char* buf, FILE* fd) {
	for (int i = 0; buf[i] != '\0'; i++) {
		fputc(toupper(buf[i]), fd);
	}
}

int main() {
	FILE* f = fopen("inp.c", "r");
	FILE* fd = fopen("out.c", "w");

	char keywords[6][10] = {"if", "int", "char", "else", "while", "break"};
	char buf[50];
	int i = 0;

	char c = fgetc(f);
	while (c != EOF) {
		if (isalpha(c)) {
			while (isalpha(c)) {
				buf[i++] = c;
				c = fgetc(f);
			}

			buf[i] = '\0';

			int flag = 0;
			for (int k = 0; k < sizeof(keywords) / sizeof(char*); k++) {
				if (strcmp(buf, keywords[k]) == 0) {
					capAll(buf, fd);
					flag = 1;
					break;
				}
			}

			if (flag == 0) {
				for (int i = 0; buf[i] != '\0'; i++) {
					fputc(buf[i], fd);
				}
			}
			i = 0;
		} else {
			fputc(c, fd);
			c = fgetc(f);
		}
	}
}
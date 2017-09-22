#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copy_string(char *ptr, char *dptr) {
	while(*ptr != '\0') {
		*dptr++ = *ptr++;
	}
	*dptr = '\0';
}

void foo(char *bar) {
	char c[12];
	copy_string(bar, c);
}

int main(char **argv, int argc) {
	char str[50];
	scanf("%s", str);
	foo(str);
	return 0;
}

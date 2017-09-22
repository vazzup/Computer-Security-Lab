#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pl() {
	printf("Hello, world\n");
}

int main() {
	printf("%p\n", &pl);
	getchar();
	return 0;
}

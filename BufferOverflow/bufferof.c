#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_string(char *ptr);

int main(char **argv, int argc) {
	char *ptr, *dptr;
	ptr = (char *) malloc(10*sizeof(char));
	dptr = (char *) malloc(10*sizeof(char));
	printf("%p %p\n", ptr, dptr);
	for(int i=0; i<10; i++) {
		*(ptr + i) = '\0';
		*(dptr + i) = '\0';
	}
	printf("Enter the string\n");
	get_string(ptr);
	printf("The dptr string is: %s\n", dptr);
	system(dptr);
	getchar();
	// free(ptr);
	// free(dptr);
	return 0;
}

void get_string(char *ptr) {
	int c;
	while(((c=getchar_unlocked()) != ' ') && (c!='\n') && (c!='\t')) {
		*ptr = c;
		ptr++;
		printf("%p\n", ptr);
	}
}

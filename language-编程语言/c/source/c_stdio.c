#include <stdio.h>

int main()
{
	// char *tmpnam(char *str)
	char buffer[L_tmpnam];
	char* ptr;

	tmpnam(buffer);
	printf("temfile 1: %s\n", buffer);

	ptr = tmpnam(NULL);
	printf("temfile 2: %s\n", ptr);

	ptr = tmpnam(NULL);
	printf("temfile 3: %s\n", ptr);

	// FILE *tmpfile(void)
	int run = 1;
	FILE *fp;

	printf("Create tmpfile\r\n");
	fp = tmpfile();
	fwrite("this is a tmpfile", 18, 1, fp);
	while (run);
	fclose(fp);

	return(0);
}

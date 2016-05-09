#include <time.h>
#include <stdio.h>
#include <stdbool.h>

void makeTest(const char *fileName, bool buffered){
	FILE *f = fopen(fileName, "wb");

	if(!buffered) setvbuf(f, NULL, _IONBF, 0);

	for (int i = 0; i < 1024 * 1024; i++){
		char c = 'A';
		fwrite(&c, sizeof(char), 1, f);
	}

	fclose(f);
}

int main(){
	clock_t a, b, c;

	a = clock();
	makeTest("PlikZBuforowaniem", true);
	b = clock();
	makeTest("PlikBezBuforowania", false);
	c= clock();

	printf("Buffered  : %u\n", b - a);
	printf("Unbuffered: %u\n", c - b);

	return 0;
}

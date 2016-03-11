#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define MAX 16384

void showTab(char[], int);
_Bool isChar(char[], int);

int main(int argc, char *argv[]){
	srand(time(NULL));
	int target, source, lbajt;
	char buf[MAX];
		
	if (argc < 2) {
		printf("\nBrak parametrow? Co ja mam z tym niby zrobic?\n\n");
		return 1;
	}
	if (argc < 3) {
		printf("\nJuż? To Wszystko? O.o Postaraj sie bardzij i podaj mi drugi parametr\n\n");
		return 1;
	}

	source = open(argv[1], O_RDONLY);

	if (source == -1) {
		printf("Blad 1\n");
		return 1;
	}

	while((lbajt = read(source, buf, MAX)) > 0) {
		char temp[4];
		for (int i = 0; i < 4; i++) temp[i] = buf[i+15];

		int liczbaElementow = sizeof(temp) / sizeof(temp[0]);
			
		if(isChar(temp, liczbaElementow)) {
			printf("Jest Charem\n");
			showTab(temp, liczbaElementow);
		}
		else{
			printf("Podana wartosc jest liczba!\n\n\n");

			for(int i = 0; i < liczbaElementow; i++) temp[i] = (rand()%10) + 48;

			target = open(argv[2], O_RDWR | O_CREAT/*| S_IRWXU*/);
			
			int ss;
			if (ss = (write(target, temp, sizeof(temp))) == -1){
				printf("Error przy zapisywaniu [%i]\n", ss);
				return 1;
			}

			if(close(source) || close(target)) printf("Blad zamkniecia pliku");
		}
	}
	return 0;
}

void showTab(char inputTab[], int n){
	for (int i = 0; i < n; i++) printf("%c", inputTab[i]);

	printf("\n");
}

_Bool isChar(char in[], int n){
	for(int i = 0; i < n; i++){
		if(in[i] > 57 || in[i] < 48) return true;
	}

	return false;
}

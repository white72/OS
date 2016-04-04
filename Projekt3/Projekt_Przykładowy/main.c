#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
//#include <stdbool.h>

#define BUFF_SIZE 16384 // 16kB -> 1/4 bufora pipe

//Do zrobienia:
//
//bool isPrime(intmax_t);
//bool isDigit(char);
//char* quickSort(char*);

int main(int argc, char** argv){
	
	if(argc < 2) { // Sprawdza czy podano argument
		puts("Nie podano pliku wejściowego. \n");
		return -2;
	}
	
	int pipeDescriptor[2];
	pipe(pipeDescriptor); //Pipuje

	if(fork()) 			//Rodzic
		if(fork()){}    //Rodzic
		else{			//Potomek 1
			FILE *file = fopen(argv[1], "r"); //Tylko do odczytu
			char* readBuffer = malloc(BUFF_SIZE);
			int loadedBytes = 0;
		
			close(pipeDescriptor[0]);
			nice(2);
			printf("Proces 1. Moja wartość nice: %d\n", nice(0));
			
			if(file == NULL) { //Sprawdza czy plik otworzono poprawnie
				puts("Błąd otwarcia pliku");
				return -3;
			}

			while (loadedBytes = fread(readBuffer, sizeof(char), BUFF_SIZE, file)){
				printf("Loaded: %d\n", loadedBytes);
				write(pipeDescriptor[1], readBuffer, loadedBytes);
			}
			
			fclose(file);

			exit(0);
		}
	else {              //Potomek 2
		char *bufferForRecivedData = malloc(BUFF_SIZE);
		int recivedBytes = 0;
		
		printf("Proces 2. Moja wartość nice: %d\n", nice(0));
		close(pipeDescriptor[1]);
	
		while(recivedBytes = read(pipeDescriptor[0], bufferForRecivedData, BUFF_SIZE)){
			printf("Second Proccess recived: %d\n", recivedBytes);
		}

		exit(0);
	}

	for (int i = 0; i < 1; i++) wait(NULL); //Dlaczego nie i < 2 ?

	return 0;
}

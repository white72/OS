#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

bool isPrime(unsigned long long);

struct valBool{
	unsigned long long value;
	//bool isPrime;
	} primeSuspected[4];

bool save(unsigned long long);
int countDigits(unsigned long long);
unsigned long long getMax(unsigned long long*, int);

int main(int argc, char* argv[]){

	if (argc < 5) {
		puts("Podaj wiecej parametrow");
		return 0;
	}

	char *pCh;
	for(int i = 0; i < (argc - 1); i++) primeSuspected[i].value = strtoull(argv[i + 1], &pCh, 10);

	pid_t pid = getpid();
	remove(".proc.o.tmp.out.tmp");
	int fileDescriptor = open(".proc.o.tmp.out.tmp", O_RDWR | O_CREAT | O_APPEND, 0700);
	for(int i = 0; i < 4; i++) {
		if(pid) pid = fork();
		
		if(!pid){
		unsigned long long valueToWrite;

		if(isPrime(primeSuspected[i].value)) valueToWrite = primeSuspected[i].value;
		else valueToWrite = 0LLU;
		
		//printf("Value %llu\n", primeSuspected[i].value);

		int digits = countDigits(valueToWrite);
		char* buff;

		buff = malloc(digits + 1);
		sprintf(buff, "%llu", valueToWrite);
		buff[digits] = (char) 10;

		 write(fileDescriptor, buff, digits + 1);
		 close(fileDescriptor);

		 free(buff);
		 
		 exit(0);
		}
	}

	for(int i = 0; i < 4; i++) wait(NULL);


	unsigned long long recivedPrimes[4];
	FILE* file = fopen(".proc.o.tmp.out.tmp", "r");

	for(int i = 0; i < 4; i++){
		char* bufferForReciveData, *oneByteBuffer;
		bufferForReciveData = malloc(sizeof(unsigned long long));
		oneByteBuffer = malloc(1);

		int savedBytesIntoBuffer = 0;

		while(fread(oneByteBuffer, sizeof(char), 1, file)){
	
			if(*oneByteBuffer == (char) 10){
				break;
			}
			
			bufferForReciveData[savedBytesIntoBuffer] = *oneByteBuffer;
			savedBytesIntoBuffer++;
		}

		recivedPrimes[i] = strtoull(bufferForReciveData, &pCh, 10);

		free(bufferForReciveData);
		free(oneByteBuffer);
		//remove(".proc.o.tmp.out.tmp");
	}

	remove(".proc.o.tmp.out.tmp");

	int primesCount = 0;

	for(int i = 0; i < 4; i++) 	if(recivedPrimes[i] > 0) primesCount++;

	printf("Znalezionych liczb pierwszych: %d\n", primesCount);

	if(primesCount > 0) save(getMax(recivedPrimes, 4));

	return 0;
}

bool isPrime(unsigned long long suspected){
	if(suspected < 2) return false;
	if(suspected == 2) return true;
	if((suspected % 2) == 0) return false;
	
	unsigned long long divisor = 3;
	unsigned long long limit = (unsigned long long) sqrtl(suspected);

	if((limit * limit) == suspected) return false;

	for(;;){
		if(divisor > limit) return true;
		if((suspected % divisor) == 0LLU) return false;
		divisor += 2LLU;
	}
	return false;
}

bool save(unsigned long long in){
	FILE* file = fopen("output.txt", "ar+");
	if(file == NULL) {
		puts("fopen ERROR");
		return false;
	}

	char* buff;//[countDigits(in)];

	buff = malloc(countDigits(in) + 1);

	sprintf(buff, "%llu", in);
	buff[countDigits(in)] = (char) 10;

	int savedBytes = fwrite(buff, sizeof(char), countDigits(in) + 1, file);

	if(savedBytes <= 0) {
		puts("Write error");
		return false;
	}
	
	if(fclose(file)) {
		puts("Close error!");
		return false;
	}

	free(buff);

	return true;
}

int countDigits(unsigned long long in){
	int result = 0;

	while(in > 0){
		result++;
		in /= 10;
	}

	return result;
}

unsigned long long getMax(unsigned long long* numbersTable, int n){
	unsigned long long result = 0;
	for(int i = 0; i < n; i++){
		if(numbersTable[i] > result) result = numbersTable[i];
	}

	return result;
}

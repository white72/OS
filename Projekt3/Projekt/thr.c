#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

bool isPrime(unsigned long long);

struct valBool{
	unsigned long long value;
	bool isPrime;
	} primeSuspected[4];

void* thread(void* inputValue){
	((struct valBool*) inputValue)->isPrime = isPrime(((struct valBool*) inputValue)->value);
	//if(((struct valBool*) inputValue)->isPrime) printf("Value: %llu\n", ((struct valBool*) inputValue)->value);
	pthread_exit(NULL);
}

bool save(unsigned long long);
int countDigits(unsigned long long);
unsigned long long getMaxPrime(struct valBool*, int);

int main(int argc, char* argv[]){

	if (argc < 5) {
		puts("Podaj wiecej parametrow");
		return 0;
	}

	char *pCh;
	pthread_t thr[4];
	for(int i = 0; i < (argc - 1); i++) primeSuspected[i].value = strtoull(argv[i + 1], &pCh, 10);
	for(int i = 0; i < 4; i++) pthread_create(&thr[i], NULL, thread, &primeSuspected[i]);

	for(int i = 0; i < 4; i++) pthread_join(thr[i], NULL);
	
	unsigned int countPrimes = 0;

	for(int i = 0; i < 4; i++) if(primeSuspected[i].isPrime) countPrimes++;

	printf("Znalezionych liczb pierwszych: %d\n", countPrimes);

	if(countPrimes > 0) save(getMaxPrime(primeSuspected, 4));

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

unsigned long long getMaxPrime(struct valBool* inputStruct, int n){
	unsigned long long result = 0;
	for(int i = 0; i < n; i++){
		if(inputStruct[i].isPrime && (inputStruct[i].value > result)) result = inputStruct[i].value;
	}

	return result;
}

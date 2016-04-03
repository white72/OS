#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>

_Bool writePid(intmax_t, int);
int countDigits(intmax_t);
int getAndRemoveFirstDigit(intmax_t*);
char* intToCharTable(intmax_t);

int main(int argc, char *argv[]){

	if(argc < 2) {
		printf("Wymagany jeden argument (plik do zapisu pid-u; plik nie musi sitniec)\n\n\n");
		return 1;
		}
	else {

	srand(time(NULL));

	pid_t pid;

	pid = fork();
	intmax_t a = rand() * getpid();
	int b = getpid()%10;

	printf("Moj pid:%d\n", (intmax_t) getpid());
	printf("Moja zmienna[a:%d b: %d]\n", a, b);

	if(!pid) {
		printf("\nJestem potomkiem :D\n\n");
		printf("\nNazwa systemu to \n");
		execl("/bin/uname", "uname -a", NULL);
	} else printf("\nNie jestem potomkiem xD\n\n");

	wait(NULL);

	printf("\nPaPa\n");
	int localDescr = open(argv[1], O_RDWR | O_CREAT | O_APPEND);

	for (int i = 0; i < 10; i++) {
		if(pid) pid = fork();
	}

	if(!writePid(getpid(), localDescr)) printf("Blad zapisu\n");	

	return 0;
	}
}

_Bool writePid(intmax_t inputPid, int desc){
	intmax_t buffSize = 32768, fileLength = 0;
	int descriptor = desc;

	if (descriptor < 0) {
		printf("Open Err\n");
		return false;
	}
	
	char *buf = malloc(buffSize);

	int tmp = 0;

	buffSize =countDigits(inputPid) + 1;
	buf = realloc(buf, buffSize * sizeof(char));
	
	char *pidTmp = intToCharTable(inputPid);
	int pidTmpSize = sizeof(pidTmp) / sizeof(pidTmp[0]);


	for (int i = 0; i < pidTmpSize; i ++) buf[i] = (char) pidTmp[i] + '0';

	buf[buffSize - 1] = (char) '\n';
	
	if(write(descriptor, buf, buffSize) == -1) {
		printf("Write Err\n");
		return false;
	}
	
	if(close(descriptor)) {
		printf("Close Err\n");
		return false;
	}

	return true;
}

int countDigits(intmax_t input) {
	int result = 0;

	while (input > 0) {
	input /= 10;
	result++;
	}

	return result;
}

int pow(int x, int y){
	if( y == 0) return 1;
	else if (x == 0) return 0;
	else {
		int result = x;
		for (int i = 1; i < y; i++) result *= x;
		return result;
	}
}

int getAndRemoveFirstDigit(intmax_t *input) {
	int tmp = *input;

	if (tmp < 10) {
		*input = NULL;
		return tmp;
	}
	else {
		tmp = (int) (tmp / (intmax_t) pow(10,(countDigits(tmp) - 1)));
		*input = *input - tmp * pow(10,(countDigits(*input) - 1));
		return tmp;
	}
}

char* intToCharTable(intmax_t input){
	int iter = countDigits(input);
	char result[iter];

	for(int i = 0; i < iter; i++) {
		result[iter-i-1] = input % 10;
		input /= 10;
	}

	return result;
}

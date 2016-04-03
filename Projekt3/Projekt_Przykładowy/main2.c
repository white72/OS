#include <unistd.h>

int main(){
	pid_t pid;
	int fd[2];

	pid = fork();

	if (pid) puts("Jestem potomkiem");
	else {
		wait(NULL);
		puts("Nie jestem potomkiem");
	}

	return 0;
}

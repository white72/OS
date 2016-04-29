#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];

    pipe(fd);

    if (fork()) ; // Rodzic
    else{ // Child 1
        if(fork()){ //Child 1
            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);

            execl("./rnd.o", "rnd.o", NULL);
        }

        else{ // Child2

            close(fd[1]);
            dup2(fd[0], 0);
            close(fd[0]);

            execl("./16zn.o", "16zn.o", NULL);
        }
    }

    for (int i = 0; i < 2; i ++) wait(NULL);

    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define TABLE_SIZE 1
#define GEO_START_VALUE 1
#define MULTIPLIER 2
#define OFF 0
#define ON 1

struct fiveBytesBox { //Struktura do zapisu 5 bajtów
    char *buffer; // Bufor na 5 bajtów
    bool varIsReady; // Czy pierwsza kolejka 5 bajtów już dotarła
    long long counter; //Licznik otrzymanych bajtów
};

void initFBB(struct fiveBytesBox *fbb) {
    fbb->varIsReady = false;
    fbb->buffer = malloc(5 * sizeof(char));
    for (int i = 0; i < 5; ++i) fbb->buffer[i] = (char) i;
    fbb->counter = 0;
}

bool isRunning(pid_t p) {
    if (!kill(p, 0)) return true;
    else return false;
}

const char *byteToBinary(char c) {
    static char b[9];
    b[0] = '\0';

    for (int z = 128; z > 0; z >>= 1) {
        strcat(b, ((c & z) == z) ? "1" : "0");
    }

    return b;
}

void addToFBB(struct fiveBytesBox *fbb, int input) { // Gdy elementSize > 1, działa poprawnie dla Little-Endian
    if (fbb->counter == INT_MAX) fbb->counter = (fbb->counter % 5) + 5; //Stack overflow


    for (int i = 3; i >= 0; i--) {
        fbb->buffer[fbb->counter] = (char) ((input >> i * 8) & 0xFF);
        fbb->counter++;
    }

    if (fbb->counter > 4) fbb->varIsReady = true;
}

bool isFiveIdenticalBytes(struct fiveBytesBox *fbb) {
    if (!fbb->varIsReady) return false;

    for (int i = 1; i < 5; i++) if (fbb->buffer[0] != fbb->buffer[i]) return false;

    return true;
}

void showFBB(struct fiveBytesBox *fbb) { //Wybisanie zawartości struktury FBB
    if (!fbb->varIsReady) return;

    for (int i = 0; i < 5; i++) {
        char c = fbb->buffer[i];
        printf("%s\n", byteToBinary(c));
    }

    printf("\n\n");
}

int main() {
    srand((unsigned int) time(NULL)); //seed

    const key_t MAIN_SHM_ID = (key_t) rand(), CHILD_MODE_SHM_ID = (key_t) rand();

    pid_t pid = fork();
    if (!pid) { //Child
        pid_t ppid = getppid();
        int mainShmId = shmget(MAIN_SHM_ID, sizeof(int) * TABLE_SIZE,
                               IPC_CREAT | 0644), childStatusShmId = shmget(
                CHILD_MODE_SHM_ID, sizeof(bool), IPC_CREAT | 0644);

        if (mainShmId == -1 || childStatusShmId == -1) {
            printf("Child process shmid error\n");
            _exit(1);
        }

        int *mainBuff = (int *) shmat(mainShmId, NULL, 0);
        bool *childModeBuff = (bool *) shmat(childStatusShmId, NULL, 0);
        childModeBuff[0] = ON;

        if (mainBuff == NULL || childModeBuff == NULL) {
            printf("Child process NULL BUF ERROR\n");
            _exit(1);
        }

        *mainBuff = GEO_START_VALUE;
        for (int i = 0; *childModeBuff && isRunning(ppid); i++) {
            if (i == INT_MAX) i = i % 5;

            mainBuff[i % TABLE_SIZE] *= MULTIPLIER;
        }

        _exit(0);
    }

    int parentMainShmId = shmget(MAIN_SHM_ID, sizeof(int) * TABLE_SIZE, IPC_CREAT | 0644),
            parentChildStatusShmId = shmget(CHILD_MODE_SHM_ID, sizeof(bool), IPC_CREAT | 0644);
    if (parentMainShmId == -1 || parentChildStatusShmId == -1) printf("Parent shmget ERROR\n");

    int *parentMainBuff = (int *) shmat(parentMainShmId, NULL, 0);
    bool *parentChildModeBuff = (bool *) shmat(parentChildStatusShmId, NULL, 0);

    if (parentMainBuff == NULL || parentChildModeBuff == NULL) printf("Parent shmat ERROR\n");

    *parentChildModeBuff = ON;

    struct fiveBytesBox *fiveBytesBox = malloc(sizeof(fiveBytesBox));
    initFBB(fiveBytesBox);

    for (int i = 0; true; i++) {
        addToFBB(fiveBytesBox, parentMainBuff[i % TABLE_SIZE]); //Odczytywanie z SHM bajtu, zapis do FBB

        if (isFiveIdenticalBytes(fiveBytesBox)) { // Jeśli cała struktura FBB zawiera takie same bajty
            printf("\n\nBINGO!!!\n");
            showFBB(fiveBytesBox);

            parentChildModeBuff[0] = OFF;
            break;
        }
    }

    return 0;
}
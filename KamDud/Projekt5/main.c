#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 16
#define CHILD_STATUS_SHM_ID 727271
#define MAIN_SHM_ID 727272
#define OFF 0
#define ON 1

struct fiveBytesBox { //Struktura do zapisu 5 bajtów
    char *buffer; // Bufor na 5 bajtów
    bool varIsReady; // Czy pierwsza kolejka 5 bajtów już dotarła
    unsigned long long counter; //Licznik otrzymanych bajtów
};

void initFBB(struct fiveBytesBox *fbb) {
    fbb->varIsReady = false;
    fbb->buffer = calloc(5, sizeof(char));
    fbb->counter = 0;
}

void addToFBB(struct fiveBytesBox *fbb, char c) {
    fbb->buffer[fbb->counter % 5] = c;
    fbb->counter++;

    if (fbb->counter > 4) fbb->varIsReady = true;
}

bool isFiveIdenticalBytes(struct fiveBytesBox *fbb) {
    if (!fbb->varIsReady) return false;

    for (int i = 1; i < 5; i++) {
        if (fbb->buffer[0] != fbb->buffer[i]) return false;
    }

    return true;
}

const char *byteToBinary(char c) {
    static char b[9];
    b[0] = '\0';

    for (int z = 128; z > 0; z >>= 1) {
        strcat(b, ((c & z) == z) ? "1" : "0");
    }

    return b;
}

void showFBB(struct fiveBytesBox *fbb) { //Wybisanie zawartości struktury FBB
    if (!fbb->varIsReady) return;

    for (int i = 0; i < 5; i++) {
        char c = fbb->buffer[i];
        printf("[%llu] %s\n", (unsigned long long) fbb->counter - 5 + i, byteToBinary(c));
    }

    printf("\n\n");
}

int main() {
    srand((unsigned int) time(NULL)); //seed

    pid_t pid = fork();
    if (!pid) { //Child
        int mainShmId = shmget(MAIN_SHM_ID, TABLE_SIZE, IPC_CREAT | 0644), childStatusShmId = shmget(
                CHILD_STATUS_SHM_ID, sizeof(bool), IPC_CREAT | 0644);

        if (mainShmId == -1 || childStatusShmId == -1) {
            printf("Child process shmid error\n");
            _exit(1);
        }

        int *mainBuff = (int *) shmat(mainShmId, NULL, 0);
        bool *statusBuff = (bool *) shmat(childStatusShmId, NULL, 0);

        if (mainBuff == NULL || statusBuff == NULL) {
            printf("Child process NULL BUF ERROR\n");
            _exit(1);
        }

        for (int i = 0; statusBuff[0] == ON; i++) mainBuff[i % TABLE_SIZE] = (char) rand() % 256; // Zapisywanie losowego bajtu, jeśli childStatus == ON

        _exit(0);
    }

    int parentMainShmId = shmget(MAIN_SHM_ID, TABLE_SIZE, IPC_CREAT | 0644), parentChildStatusShmId = shmget(
            CHILD_STATUS_SHM_ID, sizeof(bool), IPC_CREAT | 0644);
    if (parentMainShmId == -1 || parentChildStatusShmId == -1) printf("Parent shmget ERROR\n");

    int *parentMainBuff = (int *) shmat(parentMainShmId, NULL, 0);
    bool *parentChildStatusBuff = (bool *) shmat(parentChildStatusShmId, NULL, 0);
    if (parentMainBuff == NULL || parentChildStatusBuff == NULL) printf("Parent shmat ERROR\n");


    parentChildStatusBuff[0] = true;

    struct fiveBytesBox *fiveBytesBox = malloc(sizeof(fiveBytesBox));
    initFBB(fiveBytesBox);

    for (int i = 0; true; i++) {

        addToFBB(fiveBytesBox, parentMainBuff[i % TABLE_SIZE]); //Odczytywanie z SHM bajtu, zapis do FBB

        if (isFiveIdenticalBytes(fiveBytesBox)) { // Jeśli cała struktura FBB zawiera takie same bajty
            showFBB(fiveBytesBox);
            printf("BINGOOO!!!\n");
            parentChildStatusBuff[0] = OFF;
            break;
        }
    }

    wait(NULL);
    return 0;
}
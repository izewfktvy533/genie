#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main() {
    int shm;
    char* ptr;

    shm = shmget(IPC_PRIVATE, 128, IPC_CREAT|0600);

    ptr = shmat(shm, NULL, 0);

    strcpy(ptr, "HOGE");

    shmdt(ptr);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>



int main(int argc, char* argv[]) {
    int shm;
    char* ptr;
    key_t key;
    struct shmid_ds sds;

    key = ftok("./test", 0);

    shm = shmget(key, 128, IPC_CREAT|0600);

    ptr = shmat(shm, NULL, 0);

    printf("string from shared memory: %s\n", ptr);

    shmdt(ptr);

    shmctl(shm, IPC_RMID, &sds);

    return 0;
}

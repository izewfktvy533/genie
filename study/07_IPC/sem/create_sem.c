#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#define NSEMS 16


int main() {
    key_t key;
    int semid;
    unsigned short semun_array[NSEMS];
    int i;

    key = ftok("./test", 1);
    semid = semget(key, NSEMS, IPC_CREAT|0600);

    for(i=0; i<NSEMS; i++) {
        semun_array[i] = 1;
    }

    semctl(semid, NSEMS, SETALL, &semun_array);

    printf("%d\n", semid);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NSEMS 16


int main() {
    key_t key;
    int semid;
    struct sembuf sb;

    key = ftok("./test", 1);
    semid = semget(key, NSEMS, 0600);

    sb.sem_num = 1;
    sb.sem_op  = -1;
    sb.sem_flg = SEM_UNDO;

    printf("before semop()\n");

    semop(semid, &sb, 1);

    printf("after semop()\n");
    printf("press enter to exit\n");
    getchar();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define NSEMS 16


int init_sem(char* pathname, int proj_id) {
    key_t key;
    int semid;
    unsigned short semun_array[NSEMS];
    struct sembuf sb[NSEMS];
    int i;
    
    key = ftok(pathname, proj_id);
    semid = semget(key, NSEMS, 0600|IPC_CREAT|IPC_EXCL);

    if(semid >= 0) {
        for(i=0; i<NSEMS; i++) {
            semun_array[i] = 1;
        }
        
        semctl(semid, NSEMS, SETALL, &semun_array);

        printf("[pid:%d] new semaphore set, semid=%d\n", getpid(), semid);

        for(i=0; i<NSEMS; i++) {
            sb[i].sem_num = i;
            sb[i].sem_op  = -1;
            sb[i].sem_flg = SEM_UNDO;
        }

        printf("[pid:%d] before semop()\n", getpid());
        printf("[pid:%d] press enter to start semop()\n", getpid());

        getchar();

        if(semop(semid, sb, NSEMS) != 0) {
            perror("semop");
            exit(1);
        }
        
        printf("[pid:%d] press enter to exit this process\n", getpid());
        getchar();
        

        exit(0);
    }
    else {
        if(errno != EEXIST) {
            perror("semget");
            exit(1);
        }
        else {
            struct semid_ds sds;

            memset(&sds, 0, sizeof(sds));
            
            semid = semget(key, NSEMS, 0600);
            printf("[pid:%d] before semctl()\n", getpid());

            for(;;) {
                semctl(semid, 0, IPC_STAT, &sds);
                
                if(sds.sem_otime != 0) {
                    break;
                }

                printf("[pid:%d] waiting otime change...\n", getpid());
                sleep(1);
            }

            sb[0].sem_num = 0;
            sb[0].sem_op  = -1;
            sb[0].sem_flg = SEM_UNDO;

            printf("[pid:%d] before semop()\n", getpid());

            semop(semid, sb, 1);

            printf("[pid:%d] after semop()\n", getpid());
            
            semctl(semid, NSEMS, IPC_RMID, &semun_array);
        }
        
    }

    return 0;
}


int main() {
    if(init_sem("./test", 2) < 0) {
        printf("[pid:%d] init_sem() failed\n", getpid());
    }

    return 0;
}

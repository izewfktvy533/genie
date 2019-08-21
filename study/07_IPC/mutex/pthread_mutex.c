#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>


int main() {
    pthread_mutex_t* m;
    pthread_mutexattr_t mat;
    int shmid;
    pid_t pid;
    key_t key;

    key = ftok("./test", 4);
    shmid = shmget(key, sizeof(pthread_mutex_t), IPC_CREAT|0600);
    m = shmat(shmid, NULL, 0);

    pthread_mutexattr_init(&mat);
    pthread_mutexattr_setpshared(&mat, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(m, &mat);

    pid = fork();

    printf("[%s] before pthread_mutex_lock()\n", pid==0 ? "chlid" : "parent");

    pthread_mutex_lock(m);

    printf("[%s] press enter\n", pid==0 ? "chlid" : "parent");
    
    getchar();

    pthread_mutex_unlock(m);

    printf("[%s] after pthread_mutex_unlock()\n", pid==0 ? "child" : "parent");

    shmdt(m);

    if(pid != 0) {
        wait(NULL);
        printf("[%s] after wait()\n", "parent");

        shmctl(shmid, IPC_RMID, NULL);

    }

    return 0;
}

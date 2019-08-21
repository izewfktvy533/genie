#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>


int main() {
    key_t key;
    int msgid;

    key = ftok("./test", 3);

    if(key == -1) {
        perror("ftok");
        exit(1);
    }

    msgid = msgget(key, IPC_CREAT|0600);
    
    if(msgid < 0) {
        perror("msgget");
        exit(1);
    }

    printf("%d\n", msgid);

    return 0;
}

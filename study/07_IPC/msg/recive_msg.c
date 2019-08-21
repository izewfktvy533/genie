#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MTEXTSIZE 10


int main() {
    key_t key;
    int msgid, msgtype;
    struct msqid_ds mds;
    struct msgbuf {
        long mtype;
        char mtext[MTEXTSIZE];
    } mbuf;

    key = ftok("./test", 3);
    msgid = msgget(key, 0600);
    
    msgrcv(msgid, &mbuf, MTEXTSIZE, 777, 0);

    printf("%s\n", mbuf.mtext);

    msgctl(msgid, IPC_RMID, &mds);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MTEXTSIZE 10


int main() {
    key_t key;
    int msgid;
    struct msgbuf {
        long mtype;
        char mtext[MTEXTSIZE];
    } mbuf;

    key = ftok("./test", 3);
    msgid = msgget(key, 0600);

    mbuf.mtype = 777;
    memset(&mbuf.mtext, 0, MTEXTSIZE);
    strcpy(mbuf.mtext, "test");
    //mbuf.mtext[0] = 'T';

    msgsnd(msgid, &mbuf, MTEXTSIZE, 0);

    return 0;
}

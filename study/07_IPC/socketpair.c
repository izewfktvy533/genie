#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>


int main() {
    int sv[2];
    pid_t pid;
    char buf[128];

    memset(buf, 0, sizeof(buf));

    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

    pid = fork();

    if(pid == 0) {
        close(sv[1]);

        read(sv[0], buf, sizeof(buf));
        printf("child process: data from parent process [%s]\n", buf);
        
        write(sv[0], buf, sizeof(buf));
        exit(0);
    }
    else {
        int status;

        close(sv[0]);
        write(sv[1], "HOGE", 4);

        printf("parent process: child process id %d\n", pid);

        read(sv[1], buf, sizeof(buf));

        printf("%s\n", buf);

        wait(&status);

        printf("%d\n", status);
    }

    return 0;
}

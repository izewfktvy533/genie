#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


int main() {
    int p[2];
    pid_t pid;
    char buf[128];

    memset(buf, 0, sizeof(buf));

    pipe(p);

    pid = fork();

    if(pid == 0) {
        read(p[0], buf, sizeof(buf));
        printf("chlid process: data from parent process [%s]\n", buf);

        write(p[1], "HOGE HOGE", sizeof("HOGE HOGE"));

        close(p[0]);
        close(p[1]);
        
        exit(0);
    }
    else {
        int status;
        write(p[1], "HOGE", 4);
        
        sleep(1);

        read(p[0], buf, sizeof(buf));

        printf("parent process: data from chlid process [%s]\n", buf);
        wait(&status);
    
        close(p[0]);
        close(p[1]);
    }



    return 0;
}                

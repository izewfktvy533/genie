#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>

#define NEVENTS 16


void sigusr1_handler(int sig) {
    write(fileno(stdout), "signal called\n", 14);
}


int main() {
    int nfds, epfd;

    signal(SIGUSR1, sigusr1_handler);

    epfd = epoll_create(NEVENTS);

    if(epfd < 0) {
        perror("epoll_create");
        exit(1);
    }

    printf("before epoll_wait\n");

    nfds = epoll_wait(epfd, NULL, NEVENTS, -1);

    printf("after epoll_wait: %d\n", nfds);

    printf("%d\n", errno);
    perror("perror after epoll_wait");

    return 0;
}

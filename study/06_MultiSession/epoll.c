#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define NEVENTS 16
#define BUFSIZE 1024


enum mystate {
    MYSTATE_READ = 0,
    MYSTATE_WRITE
};


int main() {
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    int n, i;
    struct epoll_event ev, ev_ret[NEVENTS];
    int epfd;
    int nfds;

    struct clientinfo {
        int fd;
        char buf[BUFSIZE];
        int n;
        int state;
    };
    
    sock0 = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(sock0, (struct sockaddr*)&addr, sizeof(addr));

    listen(sock0, 5);

    epfd = epoll_create(NEVENTS);

    memset(&ev, 0, sizeof(ev));

    ev.events = EPOLLIN;
    ev.data.ptr = malloc(sizeof(struct clientinfo));

    memset(ev.data.ptr, 0, sizeof(struct clientinfo));

    ((struct clientinfo*)ev.data.ptr)->fd = sock0;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock0, &ev) != 0) {
        perror("epoll_ctl");
        exit(1);
    }

    
    for(;;) {
        printf("before epoll_wait\n");

        nfds = epoll_wait(epfd, ev_ret, NEVENTS, -1);

        printf("after epoll_wait: nfds=%d\n", nfds);

        for(i=0; i<nfds; i++) {
            struct clientinfo* ci = ev_ret[i].data.ptr;
            printf("fd=%d\n", ci->fd);

            if(ci->fd == sock0) {
                len = sizeof(client);

                sock = accept(sock0, (struct sockaddr*)&client, &len);

                printf("accept sock=%d\n", sock);

                memset(&ev, 0, sizeof(ev));

                ev.events = EPOLLIN | EPOLLONESHOT;
                ev.data.ptr = malloc(sizeof(struct clientinfo));

                memset(ev.data.ptr, 0, sizeof(struct clientinfo));
                
                ((struct clientinfo*)ev.data.ptr)->fd = sock;

                epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
            }
            else {
                if(ev_ret[i].events & EPOLLIN) {
                    ci->n = read(ci->fd, ci->buf, BUFSIZE);

                    ci->state = MYSTATE_WRITE;
                    ev_ret[i].events = EPOLLOUT;

                    epoll_ctl(epfd, EPOLL_CTL_MOD ,ci->fd, &ev_ret[i]);
                }
                else if(ev_ret[i].events & EPOLLOUT) {
                    n = write(ci->fd, ci->buf, ci->n);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, ci->fd, &ev_ret[i]);

                    close(ci->fd);

                    free(ev_ret[i].data.ptr);
                }
            }
        }
    }

    close(sock0);

    return 0;
}

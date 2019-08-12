#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define NEVENTS 16


int main() {
    struct sockaddr_in server;
    struct epoll_event ev, ev_ret[NEVENTS];
    int sock, epfd;
    char buf[1048576];
    int nfds;
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

    n = connect(sock, (struct sockaddr*)&server, sizeof(server));

    if (n != 0) {
        perror("connect");
        exit(1);
    }

    epfd = epoll_create(NEVENTS);

    if(epfd < 0) {
        perror("epoll_create");
        exit(1);
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLOUT;
    ev.data.fd = sock;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) != 0) {
        perror("epoll_ctl");
        exit(1);
    }

    int count = 1;
    while(1) {
        printf("before epoll_wait\n");

        nfds = epoll_wait(epfd, ev_ret, NEVENTS, -1);

        printf("after epill_wait\n");

        if(nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }

        if(ev_ret[0].data.fd == sock) {
            printf("%d. write %d bytes\n", count, sizeof(buf));
            count++;
            n = write(sock, buf, sizeof(buf));

            if(n <= 0) {
                printf("write error: %d\n", n);
                break;
            }
        }
    }

    close(sock);

    return 0;
}

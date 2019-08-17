#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <arpa/inet.h>

#define NEVENTS 16


int main() {
    struct sockaddr_in server;
    int sock, epfd;
    char buf[32];
    int nfds, n;
    int val;
    struct epoll_event ev, ev_ret[NEVENTS];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    inet_pton(AF_INET, "192.168.0.1", &server.sin_addr.s_addr);

    epfd = epoll_create(NEVENTS);

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) != 0) {
        perror("epoll_ctl");
        exit(1);
    }
    
    val = 1;
    ioctl(sock, FIONBIO, &val);

    n = connect(sock, (struct sockaddr*)&server, sizeof(server));

    if(n != 0) {
        if(errno == EINPROGRESS) {
            printf("before epoll_wait\n");

            nfds = epoll_wait(epfd, ev_ret, NEVENTS, 3000);

            printf("after epoll_wait\n");
        }
        else {
            perror("connect");
            exit(1);
        }

    }

    n = read(sock, buf, sizeof(buf));

    write(fileno(stdout), buf, n);

    close(sock);

    return 0;
}

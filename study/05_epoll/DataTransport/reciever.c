#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define NEVENTS 16


int main() {
    int sock1, sock2;
    struct sockaddr_in addr1, addr2;
    int epfd;
    struct epoll_event ev, ev_ret[NEVENTS];
    char buf[2049];
    int i;
    int nfds;
    int n;

    sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    sock2 = socket(AF_INET, SOCK_DGRAM, 0);

    addr1.sin_family = AF_INET;
    addr2.sin_family = AF_INET;

    inet_pton(AF_INET, "127.0.0.1", &addr1.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &addr2.sin_addr.s_addr);

    addr1.sin_port = htons(11111);
    addr2.sin_port = htons(22222);


    bind(sock1, (struct sockaddr*)&addr1, sizeof(addr1));
    bind(sock2, (struct sockaddr*)&addr2, sizeof(addr2));

    epfd = epoll_create(NEVENTS);

    if(epfd < 0) {
        perror("epoll_create");
        exit(1);
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock1;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock1, &ev) != 0) {
        perror("epoll_ctl");
        exit(1);
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock2;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock2, &ev) != 0) {
        perror("epoll_ctl");
        exit(1);
    }



    while(1) {
        printf("before epoll_wait\n\n");

        nfds = epoll_wait(epfd, ev_ret, NEVENTS, -1);

        if(nfds <= 0) {
            perror("epoll_wait");
            exit(1);
        }

        printf("after epoll_wait\n\n");

        for (i=0; i<nfds; i++) {
            memset(buf, 0, sizeof(buf));

            if(ev_ret[i].data.fd == sock1) {
                n = recv(sock1, buf, sizeof(buf)-1, 0);
                printf("%s\n", buf);
            }
            else if(ev_ret[i].data.fd == sock2) {
                n = recv(sock2, buf, sizeof(buf)-1, 0);
                printf("%s\n", buf);
            }
        }

        printf("\n");

    }
    
    close(sock1);
    close(sock2);

    return 0;
}

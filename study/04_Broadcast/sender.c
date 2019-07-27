#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define PORT 6789
#define ADDR "255.255.255.255"


int main(int argc, char* argv[]) {
    struct sockaddr_in s_addr;
    int soc;
    int fd;
    int ret;
    char buf[4097];
    ssize_t ret_read;
    ssize_t ret_write;
    int optval = 1;


    memset(buf, 0, sizeof(buf));

    if(!(argc == 2)) {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }

    if((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("opne");
        exit(1);
    }

    if((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    ret = inet_pton(AF_INET, ADDR, &s_addr.sin_addr.s_addr);
    if(ret == 0) {
        fprintf(stderr, "Not in presentation format\n");
        exit(1);
    }
    else if(ret < 0) {
        perror("inet_pton");
        exit(1);
    }


    printf("sending...\n");
    

    if(setsockopt(soc, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) != 0) {
        perror("setsocketopt");
        return 1;
    }

    while(1) {
        ret_read = read(fd, buf, sizeof(buf)-1);

        if(sendto(soc, buf, sizeof(buf)-1, 0, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
            perror("sendto");
            exit(1);
        }

        memset(buf, 0, sizeof(buf));

        if(ret_read == -1) {
            perror("read");
            exit(1);
        }
        else if(ret_read < sizeof(buf)-1) {
            break;
        }
    }


    printf("finish file transfer.\n");


    if(close(soc) < 0) {
        perror("close soc");
        exit(1);
    }
        
    if(close(fd) < 0) {
        perror("close fd");
        exit(1);
    }


    return 0;
}        

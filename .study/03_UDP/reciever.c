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
#define ADDR "127.0.0.1"


int main(int argc, char* argv[]) {
    struct sockaddr_in addr;
    struct sockaddr_in s_addr;
    int fd;
    int ret;
    int soc;
    int acc;
    int ret_recvfrom;
    socklen_t len;
    char buf[4097];

    memset(buf, 0, sizeof(buf));

    
    if((fd = open("test.json", O_WRONLY|O_CREAT, 0600)) < 0) {
        perror("open");
        exit(1);
    }

    if((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    ret = inet_pton(AF_INET, ADDR, &s_addr.sin_addr.s_addr);

    if(ret == 0) {
        fprintf(stderr, "Non in presentation format\n");
        exit(1);
    }
    else if(ret < 0) {
        perror("inet_pton");
        exit(1);
    }


    if((bind(soc, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
        perror("bind");
        exit(1);
    }

    len = sizeof(s_addr);
    int n = 0;

    while(1) {
        n += 1;
        ret_recvfrom = recvfrom(soc, buf, sizeof(buf)-1, 0, (struct sockaddr*)&s_addr, &len);
        printf("%d %d\n", n, ret_recvfrom);

        if(write(fd, buf, strlen(buf)) < 0) {
            perror("write");
            exit(1);
        }

        memset(buf, 0, sizeof(buf));

        if(ret_recvfrom < 0) {
            perror("recvfrom");
            exit(1);
        }
        else if(ret_recvfrom == 0) {
            break;
        }
    }


    printf("finish!\n");

    if(close(fd) < 0) {
        perror("close fd");
        exit(1);
    }

    if(close(soc) < 0) {
        perror("close soc");
        exit(1);
    }


    return 0;
}

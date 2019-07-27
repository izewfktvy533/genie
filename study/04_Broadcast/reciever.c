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
    int ret_write;
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
    ret = inet_pton(AF_INET, ADDR, &addr.sin_addr.s_addr);
    
    // TODO: ネットワークインタフェースを任意に選択できるようにする
    ret = 1;
    addr.sin_addr.s_addr = INADDR_ANY;


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

    while(1) {
        ret_recvfrom = recvfrom(soc, buf, sizeof(buf)-1, 0, (struct sockaddr*)&s_addr, &len);

        if((ret_write = write(fd, buf, strlen(buf))) < 0) {
            perror("write");
            exit(1);
        }

        memset(buf, 0, sizeof(buf));

        if(ret_recvfrom < 0) {
            perror("recvfrom");
            exit(1);
        }

        // FIX: データ送信の最後のパケットが失われた場合、データ受信を終えることができない 
        if(ret_write < sizeof(buf)-1) {
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

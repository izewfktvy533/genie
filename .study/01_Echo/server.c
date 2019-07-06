#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345


int main(int argc, char* argv[]) {
    struct sockaddr_in addr;
    struct sockaddr_in c_addr;
    socklen_t len;
    ssize_t ret;
    int soc;
    int acc;
    char* ip_addr;
    char buf[33];
    char msg[1024];

    memset(buf, 0, sizeof(buf));
    memset(msg, 0, sizeof(msg));

    if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if((bind(soc, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
        perror("bind");
        exit(1);
    }

    if(listen(soc, 3) < 0) {
        perror("listen");
        exit(1);
    }

    ip_addr = inet_ntoa(addr.sin_addr);
    printf("ip address: %s\n", ip_addr);
    printf("port      : %d\n", ntohs(addr.sin_port));
    printf("listen...\n");
    printf("\n");

    len = sizeof(c_addr);

    if((acc = accept(soc, (struct sockaddr*)&c_addr, &len)) < 0) {
        perror("accept");
        exit(1);
    }

    ip_addr = inet_ntoa(c_addr.sin_addr);
    printf("clieht ip address: %s\n", ip_addr);
    printf("client port      : %d\n", ntohs(c_addr.sin_port));
    printf("\n");

    while(1) {
        ret = read(acc, buf, sizeof(buf)-1);
        printf("ret: %ld\n", ret);
        
        //strncpy(msg+strlen(msg), buf, strlen(buf));
        strncat(msg, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));

        if(ret == -1) {
            perror("read");
            exit(1);
        }
        else if(ret < sizeof(buf)-1) {
            break;
        }

    }


    printf("msg: %s\n", msg);
    printf("%ld\n", strlen(msg));

    if(write(acc, msg, strlen(msg)) < 0) {
        perror("write");
        exit(1);
    }

    if(close(acc) < 0) {
        perror("close acc");
        exit(1);
    }
        

    if(close(soc) < 0) {
        perror("close soc");
        exit(1);
    }

}

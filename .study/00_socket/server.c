#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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
    int soc;
    int acc;
    char* ip_addr;

    if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       //if(errno == ) {}
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if((bind(soc, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
        //if(errno == ) {}
        perror("bind");
        exit(1);
    }
    
    if(listen(soc, 3) < 0) {
        //if(errno == ) {}
        perror("listen");
        exit(1);
    }

    ip_addr = inet_ntoa(addr.sin_addr);    
    printf("listen...\n");
    printf("address: %s\n", ip_addr);
    printf("port   : %d\n", ntohs(addr.sin_port));
    printf("\n");

    len = sizeof(c_addr);
    if((acc = accept(soc, (struct sockaddr*)&c_addr, &len)) < 0) {
        //if(errno == ) {}
        perror("accept");
        exit(1);
    }

    ip_addr = inet_ntoa(c_addr.sin_addr);    
    printf("listen...\n");
    printf("address: %s\n", ip_addr);
    printf("port   : %d\n", ntohs(c_addr.sin_port));

    if(write(acc, "HELLO", sizeof("HELLO")) < 0) {
        //if(errno == ) {}
        perror("write");
        exit(1);
    }

    if(close(acc) < 0) {
        //if(errno == ) {}
        perror("close acc");
        exit(1);
    }

    if(close(soc) < 0) {
        //if(errno == ) {}
        perror("close soc");
        exit(1);
    }

}

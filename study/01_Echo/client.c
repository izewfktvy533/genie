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
#define ADDR "127.0.0.1"


int main(int argc, char* argv[]) {
    struct sockaddr_in s_addr;
    int len;
    int soc;
    int ret;
    ssize_t ret_read;
    char buf[33];
    char msg[1024];

    memset(buf, 0, sizeof(buf));
    memset(msg, 0, sizeof(msg));

    if(!(argc == 2)) {
        fprintf(stderr, "Invalid number of arguments");
        exit(1);
    }

    if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    
    ret = inet_pton(AF_INET, ADDR, &s_addr.sin_addr.s_addr);

    if(ret == 0) {
        fprintf(stderr, "Not in presentation format");
        exit(1);
    }
    else if(ret < 0) {
        perror("inet_pton");
        exit(1);
    }

    if(connect(soc, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("argv[1]: %s\n", argv[1]);
    printf("%ld\n", strlen(argv[1]));

    if(write(soc, argv[1], strlen(argv[1])) < 0) {
        perror("write");
        exit(1);
    }

    printf("finish write\n");

    while(1) {
        ret_read = read(soc, buf, sizeof(buf)-1);
        
        strncat(msg, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));

        if(ret_read == -1) {
            perror("read");
            exit(1);
        }
        else if(ret_read < sizeof(buf)-1) {
            break;
        }

    }

    printf("msg: %s\n", msg);
    printf("%ld\n", strlen(msg));

    if(close(soc) < 0) {
        perror("close");
        exit(1);
    }

}

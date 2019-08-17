#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
    struct sockaddr_in server;
    int sock;
    char buf[1048576];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);


    n = connect(sock, (struct sockaddr*)&server, sizeof(server));

    if(n != 0) {
        perror("connect");
        exit(1);
    }
    
    int count = 1;

    while(1) {
        printf("%d. write %d bytes\n", count, sizeof(buf));

        n = write(sock, buf, sizeof(buf));
        count++;

        if(n <= 0) {
            printf("write error: %d\n", n);
            break;
        }
    }

    close(sock);

    return  0;
}

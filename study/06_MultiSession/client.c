#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main() {
    int soc;
    int buf[1024];
    int len;
    struct sockaddr_in server;

    soc = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

    connect(soc, (struct sockaddr*)&server, sizeof(server));

    write(soc, "HELLO", sizeof("HELLO"));

    len = read(soc, buf, sizeof(buf));

    printf("%s\n", buf);

    close(soc);

    return 0;
}    

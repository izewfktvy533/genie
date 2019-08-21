#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define FILEPATH "/tmp/afunix_test"


int main() {
    int soc;
    struct sockaddr_un s_un;
    socklen_t addrlen;
    char buf[1024];

    memset(buf, 0, sizeof(buf));


    soc = socket(AF_UNIX, SOCK_STREAM, 0);

    s_un.sun_family = AF_UNIX;
    strcpy(s_un.sun_path, FILEPATH);

    connect(soc, (struct sockaddr*)&s_un, sizeof(s_un));

    read(soc, buf, sizeof(buf));


    printf("%s\n", buf);

    close(soc);

    return 0;
}

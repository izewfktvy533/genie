#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define FILEPATH "/tmp/afunix_test"


int main() {
    int soc, acc_soc;
    struct sockaddr_un s_un,  acc_s_un;
    socklen_t addrlen;

    soc = socket(AF_UNIX, SOCK_STREAM, 0);

    s_un.sun_family = AF_UNIX;
    strcpy(s_un.sun_path, FILEPATH);

    bind(soc, (struct sockaddr*)&s_un, sizeof(s_un));

    listen(soc, 5);

    addrlen = sizeof(acc_s_un);
    
    printf("before accept\n");

    acc_soc = accept(soc, (struct sockaddr*)&acc_s_un, &addrlen);

    printf("after accept\n");

    write(acc_soc, "HOGE", 4);


    close(acc_soc);

    close(soc);

    unlink(FILEPATH);

    return 0;
}

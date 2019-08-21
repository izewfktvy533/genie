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
#include <stdio.h>
#include "../include/node_info.h"



int main(int argc, char* argv[]) {
    NodeInfo_t** node_info_table = create_node_info_table();
    _show_node_info_table(node_info_table);
    destroy_node_info_table(node_info_table);

    return 0;
}

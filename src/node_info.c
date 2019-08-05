#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../include/node_info.h"



NodeInfo_t** create_node_info_table() {
    FILE* fp;
    NodeInfo_t** node_info_table;
    char buf[BUF_SIZE];
    
    node_info_table = (NodeInfo_t**)malloc(sizeof(NodeInfo_t*) * TABLE_SIZE);
    memset(node_info_table, 0, sizeof(node_info_table));
    
    int i;
    for(i=0; i<TABLE_SIZE; i++) {
        node_info_table[i] = (NodeInfo_t*)malloc(sizeof(NodeInfo_t));
        memset(node_info_table[i], 0, sizeof(node_info_table[i]));
    }
    
    if((fp = fopen(INIT_NODE_INFO_FILE, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    
    i = 0;
    while(fgets(buf, sizeof(buf)+1, fp) != NULL) {
        char* addr;
        char* port;

        addr = strtok(buf,  ",");
        port = strtok(NULL, ",");
    
        update_node_info(node_info_table[i], inet_addr(addr), htons(atoi(port)));
        i++;
    }


    if(fclose(fp) == EOF) {
        perror("fclose");
        exit(1);
    }

    return node_info_table;
}



void _show_node_info_table(NodeInfo_t**  node_info_table) {
    int i;
    struct in_addr inaddr;

    for(i=0; i<TABLE_SIZE; i++) {
        inaddr.s_addr = node_info_table[i]->addr;
        printf("%s ", inet_ntoa(inaddr));
        printf("%d\n", ntohs(node_info_table[i]->port));
    }

}


void destroy_node_info_table(NodeInfo_t** node_info_table) {
    int i;

    for(i=0; i<TABLE_SIZE; i++) {
        if(node_info_table[i] == NULL) {
        }
        else {
            free(node_info_table[i]);
        }
    }

    free(node_info_table);
}



NodeInfo_t* update_node_info(NodeInfo_t* node_info, in_addr_t new_addr, in_port_t new_port) {
    node_info->addr = new_addr;
    node_info->port = new_port;

    return node_info;
}

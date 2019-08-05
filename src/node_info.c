#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/node_info.h"



NodeInfo_t** create_node_info_table() {
    NodeInfo_t** node_info_table = (NodeInfo_t**)malloc(sizeof(NodeInfo_t*) * TABLE_SIZE);
    memset(node_info_table, 0, sizeof(node_info_table));
    
    int i;
    for(i=0; i<TABLE_SIZE; i++) {
        node_info_table[i] = (NodeInfo_t*)malloc(sizeof(NodeInfo_t));
        memset(node_info_table[i], 0, sizeof(node_info_table[i]));
    }

    return node_info_table;
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

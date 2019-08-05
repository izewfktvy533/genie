#ifndef _NODE_H
#define _NODE_H

#include <netinet/in.h>

#define TABLE_SIZE 30
#define BUF_SIZE 1025
#define INIT_NODE_INFO_FILE "nodeinfo/.init_node_info"


typedef struct NodeInfo_t {
    in_addr_t addr;
    in_port_t port;
} NodeInfo_t;


NodeInfo_t** create_node_info_table();
void _show_node_info_table(NodeInfo_t** node_info_table);
void destroy_node_info_table(NodeInfo_t** node_info_table);
NodeInfo_t* update_node_info(NodeInfo_t* node_info, in_addr_t new_addr, in_port_t port);


#endif

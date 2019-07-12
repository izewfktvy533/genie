#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>


int main (int argc, const char * argv[]) {
    struct ifaddrs * if_addr_struct = NULL;
    struct ifaddrs * ifa = NULL;
    void * tmp_addr_ptr = NULL;


    if(getifaddrs(&if_addr_struct) == -1) {
        perror("getifaddrs");
        exit(1);
    }

    for (ifa=if_addr_struct; ifa!=NULL; ifa=ifa->ifa_next) {
        if(!ifa->ifa_addr) {
            continue;
        }
        if(ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmp_addr_ptr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char address_buffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmp_addr_ptr, address_buffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, address_buffer); 
        }
        else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmp_addr_ptr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char address_buffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmp_addr_ptr, address_buffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, address_buffer); 
        }
    }

    if(if_addr_struct!=NULL) {
        freeifaddrs(if_addr_struct);
    }
    
    return 0;
}

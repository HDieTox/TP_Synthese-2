#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>


#include "header/constant.h"
#include "header/utils.h"

int main(int argc, char const *argv[])
{
    if (argc != ARGS_NUMBER){
        fprintf(stderr,"Usage : gettftp <addr> <port> <filename>\n");
        exit(EXIT_FAILURE);
    }

    struct addrinfo *res;
    struct addrinfo hints;

    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    int status = getaddrinfo(argv[1],argv[2],&hints,&res);
    if (status !=0){
        fprintf(stderr,"getaddrinfo error : %s\n",gai_strerror(status));
    }

    char ip_string[128] = {0};
    char port_string[128] = {0};

    getnameinfo(res->ai_addr,res->ai_addrlen,ip_string,128,port_string,128,NI_NUMERICHOST|NI_NUMERICSERV);
    printf("%s is sresolved at : %s:%s\n", argv[1],ip_string,port_string);

    int socket_descriptor = socket(res->ai_family,res->ai_socktype,res->ai_protocol);


    char RRQ[128] = {0};
    RRQ[1] = 1;
    sprintf(RRQ+2,"%s",argv[3]);
    sprintf(RRQ+3+strlen(argv[3]),"octet");
    

    sendto(socket_descriptor,RRQ,strlen(argv[3])+9,0,res->ai_addr,res->ai_addrlen);

    char buffer_data[516];

    recvfrom(socket_descriptor,buffer_data,sizeof(char)*516,0,res->ai_addr,res->ai_addrlen);


    printf("%s",buffer_data);
    
    char ACK[4] = {0};
    ACK[1] = 4;
    ACK[2] = buffer_data[2];
    ACK[3] = buffer_data[3];

    sendto(socket_descriptor,ACK,4,0,res->ai_addr,res->ai_addrlen);


    freeaddrinfo(res);
    return 0;
}

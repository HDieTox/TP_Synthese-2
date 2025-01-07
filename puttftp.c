#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#include "header/constant.h"
#include "header/utils.h"

int main(int argc, char const *argv[])
{
    if (argc != ARGS_NUMBER){
        fprintf(stderr,"Usage : puttftp <addr> <port> <filename>\n");
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

    char ip_string[IP_SIZE] = {0};
    char port_string[IP_SIZE] = {0};

    getnameinfo(res->ai_addr,res->ai_addrlen,ip_string,128,port_string,128,NI_NUMERICHOST|NI_NUMERICSERV);
    printf("%s is resolved at : %s:%s\n", argv[1],ip_string,port_string);

    int socket_descriptor = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

    char WRQ[128] = {0};
    WRQ[1] = 2;
    sprintf(WRQ+2,"%s",argv[3]);
    sprintf(WRQ+3+strlen(argv[3]),"octet");

    sendto(socket_descriptor,WRQ,strlen(argv[3])+9,0,res->ai_addr,res->ai_addrlen);

    struct sockaddr data_connexion;
    socklen_t data_connexion_size;
    
    char buffer_data[DATA_BUFFER_SIZE] = {0};
    recvfrom(socket_descriptor,NULL,0,0,&data_connexion,&data_connexion_size);

    int file_descriptor = open(argv[3],S_IRUSR);
    read(file_descriptor,buffer_data+4,DATA_BUFFER_SIZE-4);
    buffer_data[1] = 3;
    buffer_data[3] = 1;

    sendto(socket_descriptor,buffer_data,sizeof(char)*DATA_BUFFER_SIZE,0,&data_connexion,data_connexion_size);

    return 0;
}

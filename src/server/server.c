#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <server/server.h>
#include <util.h>

int server_set_socket(struct Hostname host)
{
    int sfd;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ERR_MSG("server: cannot create socket\n\t%s\n",strerror(errno));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(host.port);
    server.sin_addr.s_addr = inet_addr(host.address);

    if (bind(sfd, (struct sockaddr*)&server, sizeof(server)) == -1)
        ERR_MSG("server: cannot bind socket\n\t%s\n", strerror(errno));

    if (listen(sfd, 8) == -1) 
        ERR_MSG("server: cannot listen to socket\n\t%s\n", strerror(errno));

    return sfd;
}

int main(int argc, char** argv)
{
    static struct option long_options[] = {
        {"hostname",    required_argument, NULL, 'H'},
    };

    struct Hostname host = { .address = NULL, .port = 0, .sock_addr = NULL };

    int opt;
    while((opt = getopt_long(argc, argv, "h:", long_options, NULL)) != -1) {
        switch(opt) {
        case 'h':
        case 'H':
            host.sock_addr= optarg;
            break;
        default: 
            ERR_MSG("server: invalid argument provided\n");
        }
    }

    if (host.sock_addr == NULL) 
        ERR_MSG("server: please specify the socket address\n");

    parse_hostname(&host);
    
    int sfd = server_set_socket(host);


    return 1;
}

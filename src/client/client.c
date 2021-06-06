#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <client/client.h>
#include <encrypt.h>
#include <serialization.h>
#include <util.h>

void parse_hostname(struct Hostname* host)
{
    size_t addr_size = 64; /* reasonable number for address */
    if ((host->address = calloc(addr_size, sizeof(char))) == NULL)
        ERR_MSG("error: malloc was not able ot allocate data");

    bool port_encountered = false;
    char *c = host->sock_addr;
    for (int i = 0; *c != '\0' || i > addr_size; i++, c++) {
        if (isalnum(*c) || *c == '.') { 
            host->address[i] = *c;
            continue;
        }

        /* everything after encountered ':' is the port */
        if (*c == ':') {
            port_encountered = true;
            host->port = atoi(c+1);
            break;
        }
    }

    if (port_encountered == false) { 
        ERR_MSG("error: please include the port in the hostname");
    }

    if (!strncmp(host->address, "localhost", 10)) {
        free(host->address);
        host->address = "127.0.0.1";
    }
}

void client_send_file(struct Hostname host, char *filename) 
{
    struct File file;
    file.name = filename;
    open_and_map_file(&file);

    /* serialize the encrypted file */
    // for (int i = 0; i < file.len; i++) {
    //     fprintf(stderr, "%d ", file.bytes[i]);
    // }

    // encrypt_msg(file.bytes, file.len);

    /* create socket and send file */
    int sfd;
    struct sockaddr_in server;

    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ERR_MSG("error: can not open socket\n\t%s", strerror(errno));
    
    memset(&server, 0, sizeof(struct sockaddr_in));

    server.sin_family = AF_INET;
    server.sin_port = htons(host.port);
    server.sin_addr.s_addr = inet_addr(host.address);

    if (connect(sfd,(struct sockaddr*)&server, sizeof(server)) == -1)
        ERR_MSG("error: can not connect to server\n\t%s", strerror(errno));

    int bytes_sent = 0;
    errno = 0;
    while ((bytes_sent += write(sfd, file.bytes, file.len)) < file.len) {
        if (errno != 0) 
            ERR_MSG("error: couldn't successfully send file\n\t%s",strerror(errno));
    }
    if (munmap(file.bytes, file.len) == -1) 
        ERR_MSG("error: could not unmap file\n\t%s\n",strerror(errno));
}

int main(int argc, char** argv)
{
    /* flags because sending and receiving is mutally exclusive */
    bool is_sending = false;
    bool is_request = false;

    char *filename = NULL;
    struct Hostname host;

    static struct option long_options[] = {
        {"hostname",    required_argument, NULL, 'H'},
        {"send",        required_argument, NULL, 'S'},
        {"request",     required_argument, NULL, 'R'},
    };

    /* allow long and short arguments */
    int opt_ind = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "h:s:r:", long_options, &opt_ind)) != -1) {
        switch (opt) {
        case 'h':
        case 'H':
            host.sock_addr = optarg;
            break;

        case 's':
        case 'S':
            is_sending = true;
            filename = optarg;
            if (is_request == true) 
                ERR_MSG("error: can not send and receive\n");

            break;

        case 'r':
        case 'R':
            is_request = true;
            filename = optarg;
            if (is_sending == true) 
                ERR_MSG("error: can not send and receive\n");

            break;
        default:
            ERR_MSG("invalid argument detected");
        }
    }

    if ((is_request | is_sending) == 0) { 
        ERR_MSG("error: specify file you are sending/receiving\n");
    } else if (host.sock_addr == NULL) {
        ERR_MSG("error: specify the hostname, you are sending/receving the file\n");
    }

    parse_hostname(&host);

    if (is_sending) {
        client_send_file(host, filename);
    } else if (is_request) {
        // client_request_file(host, filename);
    }

    return 1;
}

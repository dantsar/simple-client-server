#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <argp.h>

struct Hostname{
    char *sock_addr;
    char *address;
    int port;
};

struct File{
    char* name;
    int len;
    char* bytes;
};

// /* 
//  *  THIS IS NOT A VALID ENCRYPTION SCHEME, 
//  *  NEVER NEVER EVER EVER EVER DO THIS, 
//  *  THIS IS BADD!!!!
//  */
// char secret_key = 'a';

// char* encrypt_msg(int len, char* msg)
// {
//     return msg;
// }

// char* decrypt_msg(int len, char* msg)
// {
//     return msg;
// }

/* TODO: make this variadic, to make my life easier */
void error_and_die(const char* err_msg)
{
    fprintf(stderr, "%s\n", err_msg);
    exit(-1);
}

/* 
 * rudimentary function to parse the hostname into the address and port pair
 * e.x. localhost:8080 --> (localhost, 8080)
 * this function does not check correctness of the address, but rather just 
 * cuts the hostname when it encounteres a ':'
 */
struct Hostname* parse_hostname(struct Hostname* host)
{
    size_t addr_size = 32; /* reasonable number */
    if ( (host->address = calloc(addr_size, sizeof(char))) == NULL)
        error_and_die("error: malloc was not able ot allocate data");

    bool port_encountered = false;
    char *c = host->sock_addr;
    for (int i = 0; *c != '\0' || i > addr_size; i++, c++) {
        if (isalnum(*c) || *c == '.') { 
            host->address[i] = *c;
            continue;
        }

        /* assumes that everything after the ':' is a digit */
        if(*c == ':') {
            port_encountered = true;
            host->port = atoi(c+1);
            break;
        }
    }
    if (port_encountered == false) { 
        error_and_die("error: please include the port in the hostname");
    }

    if (!strncmp(host->address, "localhost", 10)) {
        free(host->address);
        host->address = "127.0.0.1";
    }

    return host;
}

/* 
 * opens the file provided by filename and reads it into struct File
 * kills the client if an error is encountered 
 */
void client_send_file(struct Hostname host, char *filename) 
{
    fprintf(stderr, "client_send_file: %s\n", filename);
    int fd;
    if((fd = open(filename, O_RDONLY)) == -1)
        error_and_die("client send file error: could not open file");

    /* read the file into memory */
    /* ... */
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
                error_and_die("error: can not send and receive\n");

            break;

        case 'r':
        case 'R':
            is_request = true;
            filename = optarg;
            if (is_sending == true) 
                error_and_die("error: can not send and receive\n");

            break;
        default:
            error_and_die("invalid argument detected");
        }
    }

    if ((is_request | is_sending) == 0) { 
        error_and_die("error: specify file you are sending/receiving\n");
    } else if (host.sock_addr == NULL) {
        error_and_die("error: specify the hostname, you are sending/receving the file\n");
    }


    parse_hostname(&host);
    fprintf(stdout, "parsed host:\n %s\n %s ':' %d\n", host.sock_addr, 
                                                       host.address,
                                                       host.port);

    exit(-1);

    if (is_sending) {
        client_send_file(host, filename);
    } else if (is_request) {
        // client_request_file(host, filename);
    }

    // /* encrypt contents of the file */

    return 1;
}
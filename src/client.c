#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <client.h>

/**
 *  THIS IS NOT A VALID ENCRYPTION SCHEME!!!
 *  NEVER NEVER EVER EVER EVER DO THIS!!!
 *  THIS IS BADD!!!!
 */
char secret_key = 'a';

char* encrypt_msg(char msg[], int len)
{
    char temp;
    for (int i = 0; i < len; i++) {
        temp = msg[i] ^ secret_key;
        msg[i] = temp;
    }
    return msg;
}

char* decrypt_msg(char msg[], int len)
{
    char temp;
    for (int i = 0; i < len; i++) {
        temp = msg[i] ^ secret_key;
        msg[i] = temp;
    }
    return msg;
}

/** 
 * rudimentary function to parse the hostname into the address and port pair
 * e.x. localhost:8080 --> (localhost, 8080)
 * this function does not check correctness of the address, but rather just 
 * cuts the hostname when it encounteres a ':'
 */
struct Hostname* parse_hostname(struct Hostname* host)
{
    size_t addr_size = 64; /* reasonable number for address */
    if ( (host->address = calloc(addr_size, sizeof(char))) == NULL)
        ERR_MSG("error: malloc was not able ot allocate data");

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
        ERR_MSG("error: please include the port in the hostname");
    }

    if (!strncmp(host->address, "localhost", 10)) {
        free(host->address);
        host->address = "127.0.0.1";
    }

    return host;
}

/** 
 * function opens the file and maps it into memory,
 * the file is mapped into memory to avoid the overhead of using system calls.
 */
struct File* open_and_map_file(struct File* file) 
{
    int fd;
    if ((fd = open(file->name, O_RDWR)) == -1) 
        ERR_MSG("error (client): can not open %s\ni%s\n", 
                    file->name, 
                    strerror(errno));

    struct stat file_stat;
    if(fstat(fd, &file_stat) == -1)
        ERR_MSG("error (client): error accessing stat of file %s\n %s\n", 
                                                    file->name,
                                                    strerror(errno));

    file->len = file_stat.st_size;
    if ((file->bytes = mmap(NULL, file_stat.st_size, PROT_READ | PROT_WRITE, 
                              MAP_PRIVATE, fd, 0)) == (void*)-1)
        ERR_MSG("error (client): can not map %s into memory\n\t%s\n", 
                    file->name, 
                    strerror(errno));

    return file;
}

/**
 * opens the file provided by filename and reads it into struct File
 * kills the client if an error is encountered 
 */
void client_send_file(struct Hostname host, char *filename) 
{
    struct File file;
    file.name = filename;
    open_and_map_file(&file);

    /* serialize the encrypted file */
    for (int i = 0; i < file.len; i++) {
        fprintf(stderr, "%d ", file.bytes[i]);
    }

    encrypt_msg(file.bytes, file.len);

    /* then send file over socket */

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
    // fprintf(stdout, "parsed host:\n %s\n %s ':' %d\n", host.sock_addr, 
    //                                                    host.address,
    //                                                    host.port);

    if (is_sending) {
        client_send_file(host, filename);
    } else if (is_request) {
        // client_request_file(host, filename);
    }

    return 1;
}

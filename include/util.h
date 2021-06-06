#ifndef UTIL_H
#define UTIL_H

#include <errno.h>
#include <stdio.h>

#define ERR_MSG(...)                        \
    do {                                    \
        fprintf(stderr, __VA_ARGS__);       \
        exit(-1);                           \
    } while (0); 

struct Hostname{
    char *sock_addr; /* socket address */
    char *address;
    int port;
};

struct File{
    char* name;
    unsigned int len;
    char* bytes;
};

struct Request {
    char *file;
};


/**
 * parses host with provided host.sock_addr and initalizes the remaining fields
 * with ip-address and port pair
 * 
 * @param host      only host.sock_addr is set and populates address and port
 */
void parse_hostname(struct Hostname* host);

/**
 * maps file.name into memory and initializes file.len and file.bytes
 * file is mapped into memory to avoid overhead of system calls 
 * 
 * @param file      assumes that only file.name is initialized with the desired
 *                  filename
 */
void open_and_map_file(struct File *file);

#endif 

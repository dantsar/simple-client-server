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
 * maps file.name into memory and initializes file.len and file.bytes
 * file is mapped into memory to avoid overhead of system calls 
 * 
 * @param file      assumes that only file.name is initialized with the desired
 *                  filename
 */
void open_and_map_file(struct File *file);

#endif 

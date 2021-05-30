#ifndef CLIENT_H
#define CLIENT_H

struct Hostname* parse_hostname(struct Hostname* host);
struct File* open_and_map_file(struct File *file);
void client_send_file(struct Hostname host, char *filename);
char* encrypt_msg(char msg[], int len);
char* decrypt_msg(char msg[], int len);

#define ERR_MSG(...)                        \
    do {                                    \
        fprintf(stderr, __VA_ARGS__);       \
        exit(-1);                           \
    } while (0); 

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

#endif 

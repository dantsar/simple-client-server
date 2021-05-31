#ifndef CLIENT_H
#define CLIENT_H

struct Hostname{
    char *sock_addr; /* socket address */
    char *address;
    int port;
};

struct File{
    char* name;
    int len;
    char* bytes;
};

#define ERR_MSG(...)                        \
    do {                                    \
        fprintf(stderr, __VA_ARGS__);       \
        exit(-1);                           \
    } while (0); 

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

/**
 * opens and maps filename into memory, serializes, encrypts, and sends
 * the file to the destination provided in host
 * 
 * @param host      
 * @param filename  path to the file to be sent to the host
 */
void client_send_file(struct Hostname host, char *filename);

/**
 * encrpts the provided message with a simple bitwise xor operation
 * this is a TERRIBLE encryption scheme NEVER ACTUALLY USE THIS
 * 
 * @param msg       message to be encrypted
 * @param len       length of message
 * @return          encrypted message
 */
char* encrypt_msg(char msg[], int len);

/**
 * decrypts encrypted message, but just calls encrypt message 
 * 
 * @param msg       message to be encrypted
 * @param len       length of message
 * @return          encrypted message
 */
char* decrypt_msg(char msg[], int len);

#endif 

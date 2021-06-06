#ifndef CLIENT_H
#define CLIENT_H

#include <util.h>

/**
 * parses host with provided host.sock_addr and initalizes the remaining fields
 * with ip-address and port pair
 * 
 * @param host      only host.sock_addr is set and populates address and port
 */
void parse_hostname(struct Hostname* host);

/**
 * opens and maps filename into memory, serializes, encrypts, and sends
 * the file to the destination provided in host
 * 
 * @param host      
 * @param filename  path to the file to be sent to the host
 */
void client_send_file(struct Hostname host, char *filename);

#endif 

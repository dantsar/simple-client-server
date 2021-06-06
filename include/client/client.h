#ifndef CLIENT_H
#define CLIENT_H

#include <util.h>

/**
 * opens and maps filename into memory, serializes, encrypts, and sends
 * the file to the destination provided in host
 * 
 * @param host      
 * @param filename  path to the file to be sent to the host
 */
void client_send_file(struct Hostname host, char *filename);

#endif 

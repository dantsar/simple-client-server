#ifndef SERVER_H
#define SERVER_H

#include <util.h>

/**
 * create, bind, and listen to a socket 
 * 
 * @param host      information on the server's host name
 * @return          socket file destcriptor
 */
int server_set_socket(struct Hostname host);


#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <argp.h>

struct Hostname{
    char* address;
    // char*( )
};

struct File{
    char* name;
    int len;
    char* bytes;
};

/* 
 *  THIS IS NOT A VALID ENCRYPTION SCHEME, 
 *  NEVER NEVER EVER EVER EVER DO THIS, 
 *  THIS IS BADD!!!!
 */
char secret_key = 'a';

char* encrypt_msg(int len, char* msg)
{
    return msg;
}

char* decrypt_msg(int len, char* msg)
{
    return msg;
}

int main(int argc, char** argv)
{
    if (argc <= 2) {
        fprintf(stderr,"Usage Error: %s [--hostname address:port] [--send filename] [--receive filename]", argv[0]);
    }

    return 1;
}
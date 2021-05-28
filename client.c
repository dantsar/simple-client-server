#include <stdbool.h>
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
    fprintf(stderr, "%s", err_msg);
    exit(-1);
}

int main(int argc, char** argv)
{
    static struct option long_options[] = {
        {"hostname",    required_argument, NULL, 'H'},
        {"send",        required_argument, NULL, 'S'},
        {"receive",     required_argument, NULL, 'R'},
    };

    bool is_sending = false;
    bool is_receiving = false;
    char* hostname;
    char* filename = NULL;


    int opt;
    while ((opt = getopt_long(argc, argv, "", long_options, NULL)) != -1) {
        switch (opt) {
        case 'H':
            hostname = optarg;
            break;
        case 'S':
            is_sending = true;
            filename = optarg;
            if (is_receiving == true) 
                error_and_die("error: can not send and receive\n");

            break;
        case 'R':
            is_receiving = true;
            filename = optarg;
            if (is_sending == true) 
                error_and_die("error: can not send and receive\n");

            break;
        default:
            error_and_die("invalid argument detected");
        }
    }

    if (is_receiving | is_sending == 0) { 
        error_and_die("error: specify file you are sending/receiving\n");
    } else if (hostname == NULL) {
        error_and_die("error: specify the hostname, you are sending/receving the file\n");
    }

    fprintf(stdout, "filename: %s\n", filename);


    // /* encrypt contents of the file */

    return 1;
}
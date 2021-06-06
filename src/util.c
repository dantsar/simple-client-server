#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <util.h>

void parse_hostname(struct Hostname* host)
{
    size_t addr_size = 64; /* reasonable number for address */
    if ((host->address = calloc(addr_size, sizeof(char))) == NULL)
        ERR_MSG("error: malloc was not able ot allocate data");

    bool port_encountered = false;
    char *c = host->sock_addr;
    for (int i = 0; *c != '\0' || i > addr_size; i++, c++) {
        if (isalnum(*c) || *c == '.') { 
            host->address[i] = *c;
            continue;
        }

        /* everything after encountered ':' is the port */
        if (*c == ':') {
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
}


void open_and_map_file(struct File* file) 
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
}

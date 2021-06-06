#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <util.h>

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

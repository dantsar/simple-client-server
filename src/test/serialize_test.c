#include <stdio.h>
#include <stdlib.h>

#include <serialization.h>

int main(int argc, char **argv) 
{
    int num = 300;
    int size = 16;

    char *buff, *work_head;
    buff = work_head = calloc(1, size);

    work_head = serialize_uint32(num, buff, work_head, &size);

    for (char *t = buff; t < work_head; t++)
        fprintf(stdout, "%02hhx ", *t);

    putchar('\n');
    fflush(stdout);

    int temp = deserialize_uint32(buff);
    fprintf(stdout, "decrealed: %u\n", temp);


    return 1;
}

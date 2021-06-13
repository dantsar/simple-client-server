#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <serialization.h>
#include <util.h>

void test_serialize_uint32(void) 
{
    int num = 300;

    struct Serial_Buffer buff;
    buff.size = 16;
    buff.buff = buff.head = calloc(1, buff.size);

    // buff.head = serialize_uint32(num, buff, work_head, &size);
    /* int num_read = */ serialize_uint32(num, &buff);


    /* 01 AC 02 expected */
    for (char *t = buff.buff; t < buff.head; t++)
        fprintf(stdout, "%02hhX ", *t);

    putchar('\n');
    fflush(stdout);

    unsigned int temp;
    deserialize_uint32(&temp, buff.buff);
    fprintf(stdout, "decrealed: %u\n", temp);
}

void test_serialize_bytes()
{
    struct Serial_Buffer buff;
    buff.size = 5;
    buff.buff = buff.head = calloc(1, buff.size);

    char *hello = "hello";
    size_t len = strlen(hello) + 100;

    int num_read = serialize_bytes(hello, len, &buff);
    
    for (char *c = buff.buff; c < buff.head; c++)
        fprintf(stdout, "%02hhx ", *c);

    // char bufer[256] = {0};
    char buffer[16] = {0};
    // char bufffer[256] = {0};

    unsigned int length = 0;
   /* char* head = */ deserialize_bytes(buff.buff, &length, num_read, buffer, sizeof(buffer));

    // fprintf(stdout, "head: %02hhX || %c\n", *head, *head);

    fprintf(stdout, "len:{%u}\n", length);
    fprintf(stdout, "bytes:{%s}\n", buffer);

    putchar('\n');
}

void test_serialize_File()
{
    struct Serial_Buffer buff;
    buff.size = 16;
    buff.buff = buff.head = safe_calloc(1, buff.size);

    struct File file = {.name = "aaaa", .len = 5, .bytes = "Hello"};

    int num_bytes = serialize_File(&file, &buff);

    printf("serialized bytes:\n\t");
    for (char *c = buff.buff; c < buff.head; c++)
        fprintf(stdout, "%02hhX ", *c);
    putchar('\n');

    struct File decereal = deserialize_File(buff.buff, num_bytes);

    fprintf(stdout, "name: %s\n", decereal.name);
    fprintf(stdout, "len: %d\n", decereal.len);
    fprintf(stdout, "bytes: %s\n", decereal.bytes);

    free(buff.buff);
    putchar('\n');
}

void test_serialize_Request() 
{
    struct Request req = {.file = "aaabbbccc"};
    
    struct Serial_Buffer buff; 
    buff.size = 16;
    buff.buff = buff.head = safe_calloc(1, buff.size);
    int bytes = serialize_Request(&req, &buff);

    printf("serialized bytes:\n\t");
    for (char *c = buff.buff; c < buff.head; c++)
        fprintf(stdout, "%02hhX ", *c);
    putchar('\n');

    struct Request decerealed_req = deserialize_Request(buff.buff, bytes);
    fprintf(stdout, "file: %s\n", req.file);

}

int main(int argc, char **argv) 
{
    fprintf(stdout, "test_serialize_uint32\n");
    test_serialize_uint32();

    fprintf(stdout, "test_serialize_bytes\n");
    test_serialize_bytes();

    fprintf(stdout, "test_serialize_File\n");
    test_serialize_File();

    fprintf(stdout, "test_serialize_Request\n");
    test_serialize_Request();
    return 1;
}

#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <util.h>
#include <serialization.h>

/* safe functions to prevent explosion!! */
static inline void *safe_calloc(size_t nmemb, size_t size) 
{
    void *ret;
    if ((ret = calloc(nmemb, size)) == NULL)
        ERR_MSG("could not calloc new memory\n\t%s\n", strerror(errno));
    return ret;
}

static inline void *safe_realloc(void *ptr, size_t size)
{
    void *ret;
    if ((ret = realloc(ptr, size)) == NULL)
        ERR_MSG("could not realloc\n\t%s\n", strerror(errno));
    return ret;
}

char* serialize_uint32(unsigned int ui32, char *buff_head,
                       char *work_head, int* size)
{
    /* increase buffer to prevent bad stuff */
    if ((buff_head + *size) < (work_head + 9)) {
        buff_head = safe_realloc(buff_head, *size + 9);
        *size += 9;
    }

    unsigned char chunk;
    int i = 0;
    while (ui32 >= 0x7F) {
        chunk = ((char)ui32 & 0x7F);
        chunk |= 0x80;
        buff_head[i++] = chunk;
        ui32 >>= 7;
    }
    buff_head[i++] = ui32;

    return buff_head + i;
}

unsigned int deserialize_uint32(char *work_head)
{
    unsigned int byte, num = 0;
    int i;
    for (i = 0;  work_head[i] >= 0x80u; i++) {
        byte = work_head[i] & 0x7F;
        num += byte << (7*i);
    }
    num +=  work_head[i] << (7*i);

    return num;
}

char* serialize_bytes(char* bytes, unsigned int len)
{
    
    return bytes;
}

char* deserialize_bytes(char* bytes, unsigned int len)
{

    return bytes;
}

char* serialize_File(struct File* file)
{

    return NULL;
}

char* deserialize_File(struct File* file)
{
    char *buff = safe_calloc(1, 10);
    buff = safe_realloc(buff, 16);

    int size = 16;
    char *buff_head = safe_calloc(1, size);
    char *work_head;

    // work_head = buff_head;
    // work_head = serialize_uint32(file->len, buff, work_head, &size);

    return buff;
}

char* serialize_Request(struct Request* req)
{

    return NULL;
}

char* deserialize_Request(struct Request* req)
{

    return NULL;
}

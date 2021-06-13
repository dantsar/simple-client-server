#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <util.h>
#include <serialization.h>

/*
    NOTES: 0x80 --> 1000 0000
           0x7F --> 0111 1111
*/

/* safe functions to prevent explosion!! */
void *safe_calloc(size_t nmemb, size_t size) 
{
    void *ret;
    if ((ret = calloc(nmemb, size)) == NULL)
        ERR_MSG("could not calloc new memory\n\t%s\n", strerror(errno));
    return ret;
}

void *safe_realloc(void *ptr, size_t size)
{
    void *ret;
    if ((ret = realloc(ptr, size)) == NULL)
        ERR_MSG("could not realloc\n\t%s\n", strerror(errno));
    return ret;
}

int serialize_uint32(unsigned int ui32, struct Serial_Buffer *buff)
{
    /* increase buffer to prevent bad stuff */
    if ((buff->buff + buff->size) < (buff->head + 10)) {
        buff->size += 10;
        buff->buff= safe_realloc(buff->buff, buff->size);
    }

    *(buff->head) = SERIAL_UINT32;
    buff->head++;

    unsigned char chunk;
    int i = 0;
    while (i <= 9 && ui32 >= 0x7Fu) {
        chunk = ((char)ui32 & 0x7F);
        chunk |= 0x80;
        buff->head[i++] = chunk;
        ui32 >>= 7;
    }
    buff->head[i] = ui32;
    buff->head += i+1;

    return i;
}

char* deserialize_uint32(unsigned int *num, char *bytes)
{
    if (*(bytes++) != SERIAL_UINT32)
        ERR_MSG("error deserializing bytes: expected UINT32\n");

    *num = 0;

    char byte;
    int i;
    for (i = 0;  i <= 9 && bytes[i] >= 0x80u; i++) {
        byte = bytes[i] & 0x7F;
        *num += byte << (7*i);
    }
    *num +=  bytes[i] << (7*i);

    return bytes + i + 1;
}

int serialize_bytes(char* bytes, unsigned int len, struct Serial_Buffer *buff)
{
    /* increase size of buffer to hold bytes */
    if (((buff->head - buff->buff) + len + 10) > buff->size) {
        buff->size = (buff->head - buff->buff) + len + 10;
        buff->buff = safe_realloc(buff->buff, buff->size);
    }

    *(buff->head++) = SERIAL_BYTES;
    size_t bytes_read = serialize_uint32(len, buff);

    memcpy(buff->head, bytes, len);

    buff->head += (bytes_read + len);
    return (int) bytes_read + len;
}

char* deserialize_bytes(char* bytes, unsigned int *len, int bytes_size,
                        char *buff, int buff_size) 
{
    if (*(bytes++) != SERIAL_BYTES) {
        ERR_MSG("error deserializing bytes: expected SERIAL_BYTES\n");
    }

    char *head;
    head = deserialize_uint32(len, bytes);
    
    if (*len <= buff_size && *len <= bytes_size) {
        memcpy(buff, head, *len + 1);
        return head + *len + 1;
    }

    size_t copy = bytes_size - (head - buff);
    copy = ( copy > buff_size) ? buff_size : copy;
    memcpy(buff, head, copy + 1);
    return head + copy + 1;
}

int serialize_File(struct File* file, struct Serial_Buffer *buff)
{
    if ((buff->buff - buff->head + 1) >= buff->size) {
        buff->size = (buff->buff - buff->head) + 10;
        safe_realloc(buff, buff->size);
    }

    *(buff->head++) = SERIAL_FILE;

    int bytes_read = 1;
    bytes_read = serialize_bytes(file->name, strlen(file->name), buff);

    bytes_read += serialize_bytes(file->bytes, file->len, buff);

    return bytes_read;
}

struct File deserialize_File(char *bytes, int bytes_size)
{
    if (*(bytes++) != SERIAL_FILE)
        ERR_MSG("error: expected SERIAL_FILE\n")

    struct File file;
    int name_size = 64;
    file.name = safe_calloc(1, name_size);

    char *head = bytes;

    head = deserialize_bytes(bytes, &(file.len), bytes_size, file.name, name_size);

    head = deserialize_uint32(&(file.len), head);
    file.bytes = safe_calloc(1, file.len);
    memcpy(file.bytes, head, bytes_size - (bytes - head));

    return file;
}

int serialize_Request(struct Request *req, struct Serial_Buffer *buff)
{
    *(buff->head++) = SERIAL_REQUEST;
    int bytes = serialize_bytes(req->file, strlen(req->file), buff);
    return bytes;
}

struct Request deserialize_Request(char *bytes, int bytes_size)
{
    struct Request req;
    req.file = safe_calloc(1, 64);

    if (*(bytes++) != SERIAL_REQUEST)
        ERR_MSG("error: expected SERIAL_REQUEST\n");

    unsigned int len;
    deserialize_bytes(bytes, &len, bytes_size, req.file, 64);

    return req;
}

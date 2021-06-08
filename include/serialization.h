#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <util.h>

enum DEFINE_SERIALIZATION
{
    SERIAL_UINT32=0x0,
    SERIAL_BYTES,
    SERIAL_STRING,
};

/* user defined seriaization starts from 8 (after 3 bits) */
enum USER_DEF_SERIALIZATION
{
    SERIAL_FILE=0x08,
    SERIAL_REQUEST
};

/**

 */
char* serialize_uint32(unsigned int ui32, char *buf_head,
                       char *work_head, int *size);

/**
 * 
 */
unsigned int deserialize_uint32(char *work_head);

// /** 
//  * 
//  */
// char* serialize_bytes(char* bytes, unsigned int len);

// /** 
//  * 
//  */
// char* deserialize_bytes(char* bytes, unsigned int len);


// /**
//  * 
//  */
// char* serialize_File(struct File* file);

// /**
//  * 
//  */
// char* deserialize_File(struct File* file);

// /**
//  * 
//  */
// char* serialize_Request(struct Request* req);

// /**
//  * 
//  */
// char* deserialize_Request(struct Request* req);

#endif

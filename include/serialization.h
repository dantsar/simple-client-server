#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <util.h>

#define SERIAL_UINT32   0x01u
#define SERIAL_BYTES    0x02u
// #define SERIAL_STRING   0x03u

/* user defined seriaization starts from 8 (after 3 bits) */
#define SERIAL_FILE     0x70u
#define SERIAL_REQUEST  0x71u

/*
 * A Note on the encoding used for 32 bit unsigned int:
 * the data is serialized with the least significant byte is first, 
 * and the most significant *bit* in each byte is used to determine if the 
 * following bytes encode the uint32. The remaining 7 bits are the bits from
 * the provided int and to decode it, the most significant bit is removed and
 * the order of the 7 bit chunks are reversed and concatenated together 
 *
 * EX: say we want to encode 300 (0000 0001 0010 1100). 
 * The process is: 
//  * Read the bits in 7-bit chunks and 
//  * First, reverse byte order to store LSB first
//  *  0000 0001 0010 1100 ==> 0010 1100 0000 0001
//  * Then, 
 * 
 */

void *safe_calloc(size_t nmemb, size_t size);

void *safe_realloc(void *ptr, size_t size);

/**
 * A rudimentary implementation of serializing a 32 bit unsigned int into the 
 * encoding used by Google protobufs for varints. buff is 
 * automatically realloced if the encoding exceeds the buff size
 * 
 * @param ui32      the 32 bit unsigned int
 * ...
 */
int serialize_uint32(unsigned int ui32, struct Serial_Buffer *buff);
// char* serialize_uint33(unsigned int ui32, char *buff,
//                        char *work_head, int *size);

/**
 * A rudimentary implementation of deserializing the google protobuf 
 * varint encoding
 * 
 * ...
 */
char* deserialize_uint32(unsigned int *num, char *bytes);

// /** 
//  * 
//  */
int serialize_bytes(char* bytes, unsigned int len, struct Serial_Buffer *buf);
                    //   char *buff, char *work_head, int *size);

/** 
 * populates deserailized bytes into buff and len
 * 
 * ...
 */
// this is kind of unweildy, could be better... 
char* deserialize_bytes(char* bytes, unsigned int *len, int bytes_size,
                        char *buff, int buff_size); 


/**
 * 
 */
int serialize_File(struct File* file, struct Serial_Buffer *buff);

/**
 * 
 */
struct File deserialize_File(char *bytes, int bytes_size);

/**
 * 
 */
int serialize_Request(struct Request *req, struct Serial_Buffer *buff);

/**
 * 
 */
struct Request deserialize_Request(char *bytes, int bytes_size);

#endif

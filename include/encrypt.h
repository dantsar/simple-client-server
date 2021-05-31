#ifndef ENCRYPT_H
#define ENCRYPT_H

/**
 * encrpts the provided message with a simple bitwise xor operation
 * this is a TERRIBLE encryption scheme NEVER ACTUALLY USE THIS
 * 
 * @param msg       message to be encrypted
 * @param len       length of message
 * @return          encrypted message
 */
char* encrypt_msg(char msg[], unsigned int len);

/**
 * decrypts encrypted message, but just calls encrypt message 
 * 
 * @param msg       message to be encrypted
 * @param len       length of message
 * @return          encrypted message
 */
char* decrypt_msg(char msg[], unsigned int len);

#endif


#include <encrypt.h>

/**
 * THIS IS NOT A VALID ENCRYPTION SCHEME! NEVER NEVER EVER EVER EVER DO THIS!!!
 * THIS IS BAD ON SO MANY LEVELS!!!!!!!!!!!!!!!!!!!!
 */
static char secret_key = 'a';

char* encrypt_msg(char msg[], unsigned int len)
{
    char temp;
    for (int i = 0; i < len; i++) {
        temp = msg[i] ^ secret_key;
        msg[i] = temp;
    }
    return msg;
}

char* decrypt_msg(char msg[], unsigned int len)
{
    return encrypt_msg(msg, len);
}

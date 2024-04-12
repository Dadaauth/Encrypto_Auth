#include <string.h>
#include <stdlib.h>

#include <openssl/evp.h>

#include "../main.h"
#include "tools.h"

#define ITERATIONS 10000
#define KEY_LEN 32
#define IV_LEN 16

KEYGEN generate_secrets(char *password)
{
    unsigned char key[KEY_LEN];
    unsigned char iv[IV_LEN];
    KEYGEN secret;

    secret.key = malloc(KEY_LEN);
    secret.iv = malloc(IV_LEN);
    if (secret.key == NULL || secret.iv == NULL)
        handleErrors();

    /* Generate the key */
    if (1 != PKCS5_PBKDF2_HMAC(password, strlen(password),
                        NULL, 0, ITERATIONS, EVP_sha256(),
                        KEY_LEN, key))
        handleErrors();
    /* Generate the Initialization Vector (IV) */
    if (1 != PKCS5_PBKDF2_HMAC(password, strlen(password),
                        NULL, 0, ITERATIONS, EVP_sha256(),
                        IV_LEN, iv))
        handleErrors();

    /* 
     * Copy the keys into the secret structure 
     * memcpy is used instead of just assigning key (for example)
     * to secret.key because that will only copy the memory address
     * of key which may not be valid anymore after the function closes.
    */
    memcpy(secret.key, key, KEY_LEN);
    memcpy(secret.iv, iv, IV_LEN);
    return (secret);
}

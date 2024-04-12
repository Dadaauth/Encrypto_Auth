#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <openssl/evp.h>
#include "main.h"

// Signal handler function
void sigintHandler(int sig)
{
    char answer;
    printf("\nReceived SIGINT (Ctrl+C). ");
    printf("Quiting an encryption or decryption process midway can lead to corrupted files.");
    printf("\nDo you really want to quit? (y/n): ");
    
    scanf(" %c", &answer);
    if (answer == 'y' || answer == 'Y')
        exit(EXIT_SUCCESS);
    else
    {
        signal(SIGINT, sigintHandler);
        printf("\nContinuing...\n");
    }
}

int main(int argc, char **argv)
{
    /* TODO: Handle SIGTERM SIGNAL IN CASE THE USER TRIES TO STOP THE PROGRAM. */
    unsigned char *key = "this is the key I have chosen to use for the encryption process."; /* A 256 bit key */
    unsigned char *iv = "Iv my IV";     /* A 128 bit IV */
    unsigned char plaintext[128];
    int ciphertext_len, plaintext_len;

    if (argc < 4)
    {
        fprintf(stderr, "Usage: ./encrypto <filepath> <decrypt|encrypt> <password>\n");
        exit(1);
    }

    if (signal(SIGINT, sigintHandler)  == SIG_ERR)
    {
        perror("signal");
        return (EXIT_FAILURE);
    }

    char *filepath = argv[1];
    char *operation = argv[2];
    char *password = argv[3];
    
    if (strcmp(operation, "encrypt") == 0)
        ciphertext_len = encrypt(filepath, password);
    else if (strcmp(operation, "decrypt") == 0)
        plaintext_len = decrypt(filepath, password);

    /* Cleanup Operations */
    
    return (0);
}
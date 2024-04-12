#include <string.h>
#include <signal.h>

#include <openssl/conf.h>
#include <openssl/evp.h>

#include "main.h"
#include "tools/tools.h"
#define BUFFER_SIZE 128

// Signal handler function
void encryptSigintHandler(int sig)
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
        signal(SIGINT, encryptSigintHandler);
        printf("\nContinuing...\n");
    }
}


int encrypt(char *filepath, char *password)
{
    EVP_CIPHER_CTX *ctx;
    FILE *fp; // File pointer to a file data containing the raw data.
    FILE *enc_fp; // File pointer to a file containing the encrypted data.
    unsigned char buffer[BUFFER_SIZE]; // buffer to store the read data from the file.
    unsigned char outputBuffer[BUFFER_SIZE]; // Output buffer to store the received encrypted data.
    size_t bytes_read;
    int len;
    int ciphertext_len;
    unsigned int IterCount = 0, IterNeeded = 0;
    KEYGEN secret;

    if (signal(SIGINT, encryptSigintHandler)  == SIG_ERR)
    {
        perror("signal");
        return (EXIT_FAILURE);
    }

    char *enc_filepath = prepend_filename(filepath, "ENC_");

    secret = generate_secrets(password);


    /* Message to be encrypted */
    if(!(fp = fopen(filepath, "rb")))
    {
        perror("Error opening file:");
        exit(-1);
    }
    /*
     * First open the file in write mode to
     * clear its contents, then open in append
     * mode to append data later on.
     */
    if (!(enc_fp = fopen(enc_filepath, "w")))
    {
        perror("Error opening file:");
        exit(-1);
    }
    fclose(enc_fp); // Close the file after it is opened in write mode
    if (!(enc_fp = fopen(enc_filepath, "ab")))
    {
        perror("Error opening file:");
        exit(-1);
    }

    /* Create and initialize the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /* Initialise the encryption operation. */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, secret.key, secret.iv))
        handleErrors();

    /* Calculate how many iterations will be needed for the operation. */
    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
        IterNeeded++;
    fseek(fp, 0, SEEK_SET); /* Reset the file cursor pointer to the begining of the file. */
    /* Provide the message to be encrypted, and obtain the encrypted output. */
    printf("Encrypting operation is working...\n");
    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
    {
        IterCount++;
        if (1 != EVP_EncryptUpdate(ctx, outputBuffer, &len, buffer, bytes_read))
            handleErrors();
        ciphertext_len += len;
        // BIO_dump_fp(stdout, (const char*)outputBuffer, len);
        fwrite(outputBuffer, 1, len, enc_fp);
        drawProgressBar(((double)IterCount / IterNeeded) * 100);
    }

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    printf("Finalising encryption process...\n");
    if (1 != EVP_EncryptFinal_ex(ctx, outputBuffer, &len))
        handleErrors();
    // BIO_dump_fp(stdout, (const char*)outputBuffer, len);
    fwrite(outputBuffer, 1, len, enc_fp);
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    fclose(fp);
    fclose(enc_fp);
    free(enc_filepath);

    return (ciphertext_len);
}

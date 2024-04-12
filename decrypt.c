#include <string.h>
#include <signal.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "main.h"
#include "tools/tools.h"
#define BUFFER_SIZE 128

// Signal handler function
void decryptSigintHandler(int sig)
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
        signal(SIGINT, decryptSigintHandler);
        printf("\nContinuing...\n");
    }
}


int decrypt(char *filepath, char *password)
{
    EVP_CIPHER_CTX *ctx;

    FILE *fp; // File pointer to a file data containing the raw data.
    FILE *enc_fp; // File pointer to a file containing the encrypted data.
    unsigned char buffer[BUFFER_SIZE]; // buffer to store the read data from the file.
    unsigned char outputBuffer[BUFFER_SIZE]; // Output buffer to store the received decrypted data.
    size_t bytes_read;
    int len;
    int plaintext_len;
    int IterCount = 0, IterNeeded = 0;
    KEYGEN secret;

    if (signal(SIGINT, decryptSigintHandler)  == SIG_ERR)
    {
        perror("signal");
        return (EXIT_FAILURE);
    }

    char *enc_filepath = prepend_filename(filepath, "ENC_");
    char *dec_filepath = prepend_filename(filepath, "DEC_");

    secret = generate_secrets(password);

    /* Message to be decrypted */
    if(!(enc_fp = fopen(enc_filepath, "rb")))
    {
        perror("Error opening file:");
        exit(-1);
    }
    /*
     * First open the file in write mode to
     * clear its contents, then open in append
     * mode to append data later on.
     */
    if (!(fp = fopen(dec_filepath, "w")))
    {
        perror("Error opening file:");
        exit(-1);
    }
    fclose(fp); // Close the file after it is opened in write mode
    if (!(fp = fopen(dec_filepath, "ab")))
    {
        perror("Error opening file:");
        exit(-1);
    }

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, secret.key, secret.iv))
        handleErrors();

    /* Calculate how many iterations will be needed for the operation. */
    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, enc_fp)) > 0)
        IterNeeded++;
    fseek(enc_fp, 0, SEEK_SET); /* Reset the file cursor pointer to the begining of the file. */
    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    printf("Decrypting operation is working...\n");
    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, enc_fp)) > 0)
    {
        IterCount++;
        if (1 != EVP_DecryptUpdate(ctx, outputBuffer, &len, buffer, bytes_read))
            handleErrors();
        plaintext_len += len;
        // BIO_dump_fp(stdout, (const char*)outputBuffer, len);
        fwrite(outputBuffer, 1, len, fp);
        drawProgressBar(((double)IterCount / IterNeeded) * 100);
    }

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    printf("Finalising decryption process...\n");
    if (1 != EVP_DecryptFinal_ex(ctx, outputBuffer, &len))
    {
        fprintf(stderr, "An error occured. You should probably check your password.....\n");
        handleErrors();
    }
    // BIO_dump_fp(stdout, (const char*)outputBuffer, len);
    fwrite(outputBuffer, 1, len, fp);
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    fclose(fp);
    fclose(enc_fp);
    free(dec_filepath);
    free(enc_filepath);

    return (plaintext_len);
}


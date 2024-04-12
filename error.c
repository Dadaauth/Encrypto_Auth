#include <openssl/err.h>
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}
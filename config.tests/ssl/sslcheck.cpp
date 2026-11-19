#include <openssl/ssl.h>

int main(int argc, char **argv)
{
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_method());
    if (!ctx)
        return 1;
    SSL_CTX_free(ctx);
    return 0;
}

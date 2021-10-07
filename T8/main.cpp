#include <cstdlib>
#include <cstring>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/pkcs7.h>
#include <openssl/rand.h>
#include <openssl/x509.h>

#define BUFFER_SIZE 4096

const char cacert[] = "\
-----BEGIN CERTIFICATE----- \n\
MIIB/zCCAaagAwIBAgIJAKKa0PAt9M1FMAoGCCqBHM9VAYN1MFsxCzAJBgNVBAYT \n\
AkNOMQ4wDAYDVQQIDAVIdUJlaTEOMAwGA1UEBwwFV3VIYW4xDTALBgNVBAoMBEhV \n\
U1QxDDAKBgNVBAsMA0NTRTEPMA0GA1UEAwwGY2Fyb290MB4XDTIwMDkyMDIwNTkx \n\
OVoXDTMwMDkxODIwNTkxOVowWzELMAkGA1UEBhMCQ04xDjAMBgNVBAgMBUh1QmVp \n\
MQ4wDAYDVQQHDAVXdUhhbjENMAsGA1UECgwESFVTVDEMMAoGA1UECwwDQ1NFMQ8w \n\
DQYDVQQDDAZjYXJvb3QwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAASJ8mm28JJR \n\
bZKLr6DCo1+KWimpKEsiTfZM19Zi5ao7Au6YLosyN71256MWmjwkwXxJeLa0lCfm \n\
kF/YWCX6qGQ0o1MwUTAdBgNVHQ4EFgQUAL5hW3RUzqvsiTzIc1gUHeK5uzQwHwYD \n\
VR0jBBgwFoAUAL5hW3RUzqvsiTzIc1gUHeK5uzQwDwYDVR0TAQH/BAUwAwEB/zAK \n\
BggqgRzPVQGDdQNHADBEAiAaZMmvE5zzXHx/TBgdUhjtpRH3Jpd6OZ+SOAfMtKxD \n\
LAIgdKq/v2Jkmn37Y9U8FHYDfFqk5I0qlQOAmuvbVUi3yvM= \n\
-----END CERTIFICATE----- \n\
";

const char pkeyB[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEINQhCKslrI3tKt6cK4Kxkor/LBvM8PSv699Xea7kTXTToAoGCCqBHM9V \n\
AYItoUQDQgAEH7rLLiFASe3SWSsGbxFUtfPY//pXqLvgM6ROyiYhLkPxEulwrTe8 \n\
kv5R8/NA7kSSvcsGIQ9EPWhr6HnCULpklw== \n\
-----END EC PRIVATE KEY----- \n\
";

X509 *get_X509(const char *cert) noexcept {
    BIO *bio = BIO_new(BIO_s_mem());
    BIO_puts(bio, cert);
    return PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
}

EVP_PKEY *get_pkey(const char *private_key) noexcept {
    auto bio_pkey = BIO_new_mem_buf((char *) private_key, int(strlen(private_key)));
    return bio_pkey ? PEM_read_bio_PrivateKey(bio_pkey, nullptr, nullptr, nullptr) : nullptr;
}

PKCS7 *get_PKCS7(const char *pkcs7) noexcept {
    auto p7bio = BIO_new_mem_buf((char *) pkcs7, int(strlen(pkcs7)));
    return p7bio ? PEM_read_bio_PKCS7(p7bio, nullptr, nullptr, nullptr) : nullptr;
}

int check_sig(PKCS7 *p7, BIO *p7_bio, X509 *ca_cert) noexcept {
    STACK_OF(PKCS7_SIGNER_INFO) *signer_info = PKCS7_get_signer_info(p7);
    if (!signer_info)
        return false;
    X509_STORE *store = X509_STORE_new();
    X509_STORE_CTX *store_ctx_new = X509_STORE_CTX_new();
    X509_STORE_add_cert(store, ca_cert);
    int sign_num = sk_PKCS7_SIGNER_INFO_num(signer_info);
    bool flag = true;
    for (int i = 0; i < sign_num; i++) {
        PKCS7_SIGNER_INFO *sign_info = sk_PKCS7_SIGNER_INFO_value(signer_info, i);
        auto pkcs7_data_verify = PKCS7_dataVerify(store, store_ctx_new, p7_bio, p7, sign_info);
        PKCS7_SIGNER_INFO_free(sign_info);
        if (pkcs7_data_verify <= 0) {
            flag = false;
            break;
        }
    }
    X509_STORE_free(store);
    X509_STORE_CTX_free(store_ctx_new);
    sk_PKCS7_SIGNER_INFO_free(signer_info);
    return flag;
}

void error_handler(bool status) noexcept {
    if (status) {
        printf("ERROR\n");
        exit(0);
    }
}

int main() noexcept {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    char temp_buffer[BUFFER_SIZE];
    char input_data[BUFFER_SIZE];
    PKCS7 *p7;
    X509 *cert;
    BIO *p7_bio;
    BIO *out = BIO_new_fd(fileno(stdout), BIO_NOCLOSE);
    fread(input_data, sizeof(input_data) - 1, 1, stdin);
    p7 = get_PKCS7(input_data);
    error_handler(!p7);
    p7_bio = PKCS7_dataDecode(p7, get_pkey(pkeyB), nullptr, nullptr);
    error_handler(!p7_bio);
    cert = get_X509(cacert);
    error_handler(!cert);
    int len = BIO_read(p7_bio, temp_buffer, sizeof(temp_buffer));
    error_handler(len <= 0);
    error_handler(!check_sig(p7, p7_bio, cert));
    BIO_write(out, temp_buffer, len);
    X509_free(cert);
    return 0;
}
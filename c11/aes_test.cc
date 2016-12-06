/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 12/05/16 10:38:23 CST
*/

//$ g++ aes_test.cc -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <cstdio>
#include <string>

/*
std::string encode(EVP_CIPHER_CTX *ctx, const char *input, size_t input_len) {
    std::string result;
    result.resize(input_len);
    int offset = 0;
	int bufsize = 128;
    int remaining = input_len;

    int len = 0;
    while (remaining > bufsize) {
        if (!EVP_CipherUpdate (ctx, (unsigned char*)&result[offset], &len, (const unsigned char*)input+offset, bufsize)) {
            std::string err = ERR_lib_error_string(ERR_get_error());
			return err;
        }
        offset += len;
        remaining -= len;
    }
    if (remaining) {
        if (!EVP_CipherUpdate (ctx, (unsigned char*)&result[offset], &len, (const unsigned char*)input+offset, remaining)) {
            std::string err = ERR_lib_error_string(ERR_get_error());
			return err;
        }
    }
    return result;
}
*/

std::string encode(EVP_CIPHER_CTX *ctx, const char *input, size_t input_len) {
    std::string result;
    result.resize(input_len);

    int len = 0;
	if (!EVP_CipherUpdate (ctx, (unsigned char*)&result[0], &len, (const unsigned char*)input, input_len)) {
		std::string err = ERR_lib_error_string(ERR_get_error());
		return err;
	}
    return result;
}

int main() {
	unsigned char key[] = "1234567890aaaaaa";
	unsigned char iv[] = "1234567890123456";
	
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);		

	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!EVP_CipherInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv, 1)) {
		return -1;	
	}
	EVP_CIPHER_CTX_set_padding(ctx, 0);
	
	char data[18] = "1234567890abcdfgh";
	std::string result = encode(ctx, data, sizeof(data));
	
	std::cout<<result.length()<<std::endl;
	std::cout<<result<<std::endl;

	EVP_CIPHER_CTX_free(ctx);
	return 0;
}

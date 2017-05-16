#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

int main(int argc, char **argv)
{

  EVP_CIPHER_CTX en;
  EVP_CIPHER_CTX de;
  EVP_CIPHER_CTX_init(&en);
  EVP_CIPHER_CTX_init(&de);
  const EVP_CIPHER *cipher_type;
  unsigned char *passkey, *passiv, *plaintxt;
  unsigned char *plaintext = NULL;
  unsigned char *ciphertext = NULL;
  int input_len = 0;

  unsigned char iv[] = { 0x00, 0x01, 0x02, 0x03,
                         0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0a, 0x0b,
                         0x0c, 0x0d, 0x0e, 0x0f };

  unsigned char key[] = { 0x2b, 0x7e, 0x15, 0x16,
                          0x28, 0xae, 0xd2, 0xa6,
                          0xab, 0xf7, 0x15, 0x88,
                          0x09, 0xcf, 0x4f, 0x3c };

  const char *string_to_encrypt = "hi this is patrick immling\n'Doctor'.\n'Doctor' who ?\nPrecisely! 123910!§$$§% !%%$&$(/=))?=(#ü++Ü**<,.here we go sometimes it i s difficult but 187! 1$5 78@2 14  .TӒ��틪�ձ1z.$�?�U���<y";

    cipher_type = EVP_aes_128_cbc();

    EVP_EncryptInit_ex(&en, cipher_type, NULL, key, iv);
    EVP_DecryptInit_ex(&de, cipher_type, NULL, key, iv);

    // The data we want to encrypt is a string.  So we can just do a simple
    // strlen to calculate its length.  But the encrypted buffer is going to
    // be padded with PKCS padding.  In the worst case, our string is a
    // multiple of the AES block size (16 bytes).  In that case, the PKCS
    // padding will be an additional 16 bytes after our data.  So we could
    // precisely calculate the buffer with this:
    // int input_len = strlen(string_to_encrypt);
    // malloc( input_len + 16 - (input_len % 16) );
    // But why get fancy?  Just add an extra AES block and have at most 16
    // unused bytes at the end, and usually less than that.
    static const int MAX_PADDING_LEN = 16;

    // We add 1 because we're encrypting a string, which has a NULL terminator
    // and want that NULL terminator to be present when we decrypt.
    input_len = strlen(string_to_encrypt) + 1;
    ciphertext = (unsigned char *) malloc(input_len + MAX_PADDING_LEN);

      /* allows reusing of 'e' for multiple encryption cycles */
      if(!EVP_EncryptInit_ex(&en, NULL, NULL, NULL, NULL)){
        printf("ERROR in EVP_EncryptInit_ex \n");
        return 1;
      }

      // This function works on binary data, not strings.  So we cast our
      // string to an unsigned char * and tell it that the length is the string
      // length + 1 byte for the null terminator.
      int bytes_written = 0;
      int ciphertext_len = 0;
      if(!EVP_EncryptUpdate(&en,
                           ciphertext, &bytes_written,
                           (unsigned char *) string_to_encrypt, input_len) ) {
        printf("ERROR in EVP_EncryptUpdate \n");
        return 1;
      }
      ciphertext_len += bytes_written;

      // Right now the ciphertext buffer contains only the encrypted version
      // of the input data up to the last full AES block.  E.g., if your input
      // size is 206, then ciphertext_len will be 192 because you have 14 bytes
      // left to encrypt and those bytes can't fill a full AES block.  But the
      // encryptor has stored those bytes and is waiting either for more bytes
      // or the call to EVP_EncryptFinal where it will add padding to make the
      // encrypted data the same size as the AES block (i.e., 2 bytes of padding
      // in the above example).
      printf("Input len: %d, ciphertext_len: %d\n", input_len, ciphertext_len);

      // EVP_EncryptFinal_ex writes the padding.  The whole point of the
      // bytes_written variable from EVP_EncryptUpdate is to tell us how much
      // of the buffer is full so we know where we can write the padding.
      // Note that we know our buffer is large enough so we're not bothering to
      // keep track of the buffer size.  We just keep track of how much data is
      // in it.

      if(!EVP_EncryptFinal_ex(&en,
                              ciphertext + bytes_written,
                              &bytes_written)){
        printf("ERROR in EVP_EncryptFinal_ex \n");
        return 1;
      }
      ciphertext_len += bytes_written;

      EVP_CIPHER_CTX_cleanup(&en);

      printf("Input len: %d, ciphertext_len: %d\n", input_len, ciphertext_len);

      // We'll pretend we don't know the input length here.  We do know that
      // the ciphertext length is at most 16 bytes + the input length.  So
      // since the ciphertext is always greater than the input length, we can
      // declare plaintext buffer size = ciphertext buffer size and know that
      // there's no way we'll overflow our plaintext buffer.  It will have at
      // most 16 bytes of wasted space on the end, but that's ok.
      plaintext = (unsigned char *) malloc(ciphertext_len);

      if(!EVP_DecryptInit_ex(&de, NULL, NULL, NULL, NULL)){
        printf("ERROR in EVP_DecryptInit_ex \n");
        return 1;
      }

      // No!  You're encrypting arbitrary data, so you should use padding.  You
      // don't use padding only if you know in advance that you're encrypting
      // data whose length is a multiple of the block size.  Like when running
      // the AES known-answer tests.
      // EVP_CIPHER_CTX_set_padding(&de, 0); /* no! */

      int plaintext_len = 0;
      if(!EVP_DecryptUpdate(&de,
                            plaintext, &bytes_written,
                            ciphertext, ciphertext_len)){
        printf("ERROR in EVP_DecryptUpdate\n");
        return 1;
      }
      plaintext_len += bytes_written;

      // This function verifies the padding and then discards it.  It will
      // return an error if the padding isn't what it expects, which means that
      // the data was malformed or you are decrypting it with the wrong key.
      if(!EVP_DecryptFinal_ex(&de,
                              plaintext + bytes_written, &bytes_written)){
        printf("ERROR in EVP_DecryptFinal_ex\n");
        return 1;
      }
      plaintext_len += bytes_written;

      EVP_CIPHER_CTX_cleanup(&de);

      // We encrypted a string, so we know that we decrypted a string.  So we
      // can just print it.  Note that we know our binary data is a string so
      // we just cast it to a char *.  We could just have easily declared it
      // originally as a char * (I think I changed that from your original
      // program, actually) and then cast it in the call to EVP_DecryptUpdate.
      printf("input_len: %d, ciphertext_len: %d, plaintext_len: %d\n",
          input_len, ciphertext_len, plaintext_len);

      printf("Decrypted value = %s\n", plaintext);
      if( strcmp(string_to_encrypt, (char *) plaintext) == 0 ) {
          printf("Decrypted data matches input data.\n");
      }
      else {
          printf("Decrypted data does not match input data.\n");
      }

   free(ciphertext);
   free(plaintext);

  return 0;
}/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 12/06/16 11:06:13 CST
*/



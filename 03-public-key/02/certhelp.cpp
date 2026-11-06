#include <iostream>
#include <string>
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/decoder.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/core_names.h>
#include <openssl/x509.h>
#include <gmpxx.h>

static termios DisableEcho()
{
  termios t_old, t_new;
  tcgetattr(STDIN_FILENO, &t_old);
  t_new = t_old;
  t_new.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
  return t_old;
}

static void EnableEcho(const termios &old)
{
  tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

static void PrintLastOpenSSLError()
{
  char buf[1024] = {0};
  ERR_error_string_n(ERR_peek_error(), buf, sizeof(buf));
  std::cerr << buf << std::endl;
}

static int ProvidePassword(char* pass, size_t pass_size, size_t *pass_len, const OSSL_PARAM[], void*)
{
  std::cout << "Please enter passphrase: ";
  std::string s;
  const termios old = DisableEcho();
  std::cin >> s;
  EnableEcho(old);
  std::cout << std::endl;
  strncpy(pass, s.c_str(), pass_size);
  *pass_len = s.length();
  return 1; //1 means success for OpenSSL
}

static int ReadKeyFromPEMFile(const char * const pem_filename, EVP_PKEY ** const pkey)
{
  FILE * const fp = fopen(pem_filename, "r");
  if(!fp)
  {
    std::cerr << "File " << pem_filename << " not found" << std::endl;
    return 1;
  }
  OSSL_DECODER_CTX * const dctx = OSSL_DECODER_CTX_new_for_pkey(pkey, "PEM", nullptr, "RSA", OSSL_KEYMGMT_SELECT_KEYPAIR, nullptr, nullptr);
  if (!dctx)
  {
    std::cerr << "An error occurred while initializing the RSA decoder" << std::endl;
    PrintLastOpenSSLError();
    return 2;
  }
  if (!OSSL_DECODER_CTX_set_passphrase_cb(dctx, ProvidePassword, nullptr))
  {
    std::cerr << "An error occurred while setting the password" << std::endl;
    PrintLastOpenSSLError();
    return 3;
  }
  const int ret = OSSL_DECODER_from_fp(dctx, fp);
  OSSL_DECODER_CTX_free(dctx);
  if (ret != 1)
  {
    std::cerr << "An error occurred while decoding the PEM file" << std::endl;
    PrintLastOpenSSLError();
    return 4;
  }
  return 0;
}

static int ReadKeyFromCertificate(const char * const crt_filename, EVP_PKEY ** const pkey)
{
  FILE * const fp = fopen(crt_filename, "r");
  if(!fp)
  {
    std::cerr << "File " << crt_filename << " not found" << std::endl;
    return 1;
  }
  const X509 * const certificate = PEM_read_X509(fp, nullptr, nullptr, nullptr);
  fclose(fp);
  if (!certificate)
  {
    std::cerr << "An error occurred while reading the certificate" << std::endl;
    PrintLastOpenSSLError();
    return 2;
  }
  *pkey = X509_get0_pubkey(certificate);
  if (!*pkey)
  {
    std::cerr << "An error occurred while reading the public key from the certificate" << std::endl;
    PrintLastOpenSSLError();
    return 3;
  }
  //std::cout << "PK bits: " << EVP_PKEY_bits(*pkey) << std::endl;
  return 0;
}

static void ConvertBigNum(const BIGNUM * const value, mpz_class &converted_value)
{
  char * const decimal_value = BN_bn2dec(value);
  converted_value.set_str(decimal_value, 10);
  OPENSSL_free(decimal_value);
}

bool ReadPrivateKeyFromFile(const char * const pem_filename, mpz_class &d, mpz_class &N)
{
  EVP_PKEY *pkey = nullptr;
  if (ReadKeyFromPEMFile(pem_filename, &pkey))
    return false;
  BIGNUM *big_d = nullptr, *big_N = nullptr;
  if (!EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_RSA_D, &big_d) || !big_d)
  {
    std::cerr << "An error occured while reading d" << std::endl;
    PrintLastOpenSSLError();
    return false;
  }
  if (!EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_RSA_N, &big_N) || !big_N)
  {
    std::cerr << "An error occured while reading N" << std::endl;
    PrintLastOpenSSLError();
    return false;
  }
  ConvertBigNum(big_d, d);
  ConvertBigNum(big_N, N);
  return true;
}

bool ReadPublicKeyFromFile(const char * const crt_filename, mpz_class &e, mpz_class &N)
{
  EVP_PKEY *pkey = nullptr;
  if (ReadKeyFromCertificate(crt_filename, &pkey))
    return false;
  BIGNUM *big_e = nullptr, *big_N = nullptr;
  if (!EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_RSA_E, &big_e) || !big_e)
  {
    std::cerr << "An error occured while reading e" << std::endl;
    PrintLastOpenSSLError();
    return false;
  }
  if (!EVP_PKEY_get_bn_param(pkey, OSSL_PKEY_PARAM_RSA_N, &big_N) || !big_N)
  {
    std::cerr << "An error occured while reading N" << std::endl;
    PrintLastOpenSSLError();
    return false;
  }
  ConvertBigNum(big_e, e);
  ConvertBigNum(big_N, N);
  return true;
}


#pragma once

#include <gmpxx.h>

bool ReadPrivateKeyFromFile(const char * const pem_filename, mpz_class &d, mpz_class &N);
bool ReadPublicKeyFromFile(const char * const crt_filename, mpz_class &e, mpz_class &N);

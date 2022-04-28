#ifndef MILLER_RABIN_H_
#define MILLER_RABIN_H_

#include <gmp.h>
#include "utils.h"

// Implementation du test de Miller Rabin.
int test_miller_rabin(mpz_t n, size_t k);

void execute_test_miller_rabin(mpz_t n, size_t k, args_opt opt);

#endif // MILLER_RABIN_H_
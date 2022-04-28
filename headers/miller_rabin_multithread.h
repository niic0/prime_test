#ifndef MILLER_RABIN_MULTITHREAD_H_
#define MILLER_RABIN_MULTITHREAD_H_

#include <gmp.h>
#include "utils.h"

typedef struct n_and_k {
    size_t k;
    mpz_t n;
} n_and_k;

void *iter_miller(void *args);
void *miller_rabin_multithread(void *args);
int execute_miller_rabin_multithread(mpz_t n, size_t k, args_opt opt, size_t NUM_THREADS);

#endif // MILLER_RABIN_MULTITHREAD_H_
#ifndef FERMAT_H_
#define FERMAT_H_

#include <gmp.h>
#include "utils.h"

// Implementation du test de Fermat.
int test_fermat(mpz_t n, size_t k);

void execute_test_fermat(mpz_t n, size_t k, args_opt opt);

#endif // FERMAT_H_
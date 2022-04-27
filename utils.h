#ifndef UTILS_H_
#define UTILS_H_

#include <gmp.h>

// Options possibles pour les arguements
typedef struct args_opt
{
    int flag_n;
    int flag_k;
    int flag_mr; // miller rabin
    int flag_f;  // fermat
    int flag_t;  // time
    mpz_t n;
    size_t k;
} args_opt;


// Implementation de l'algorithme Square and Multiply appliquee au cas de
// l'exponentiation modulaire ou r = a^h mod n.
void square_multiply(mpz_t r, mpz_t a, mpz_t h, mpz_t n);

// Prend un nombre au hasard dans un ensemble tel que lower < r < upper.
void range_rand(mpz_t r, mpz_t lower, mpz_t upper);

// Decompose n en 2^s * t .
void decompose(mpz_t n,mpz_t s,mpz_t t);

// Permet de gerer l'entree inserer par l'utilisateur
int handle_args(int argc, char** argv, args_opt* opt);

// Affiche l'utilisation du programme
void usage();

#endif // UTILS_H_
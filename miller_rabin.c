#include "./headers/miller_rabin.h"

/*
 * Fonction: test_miller_rabin
 * ---------------------------
 *  Implementation de l'algorithme du test de Miller Rabin.
 *
 *  n: Nombre a tester
 *  k: Nombre de repetitions voulue
 *
 *  retour: Retourne 1 si n est premier, 0 si il est compose. Si le k est trop
 *          petit, le resultat est moins probable et donc moins fiable.
 */
int test_miller_rabin(mpz_t n, size_t k)
{
    if (mpz_get_ui(n) % 2 == 0)
    {
        if (mpz_cmp_ui(n, 2) == 0)
            return 1;
        else
            return 0;
    }

    mpz_t s, t, y, a, one, two, min_one, n_min_one, one_mod_n, min_one_mod_n;

    mpz_inits(s, t, y, a, min_one_mod_n, one_mod_n, n_min_one, NULL);
    mpz_init_set_si(one, 1);
    mpz_init_set_si(two, 2);
    mpz_init_set_si(min_one, -1);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_mod(one_mod_n, one, n);
    mpz_mod(min_one_mod_n, min_one, n);
    mpz_sub(n_min_one, n, one);

    decompose(n_min_one, s, t);

    for (size_t i = 0; i < k; i++)
    {
        mpz_urandomm(a, state, n_min_one);
        square_multiply(y, a, n, t);

        if ((mpz_cmp_si(y, 1) != 0) && (mpz_cmp(y, min_one_mod_n) != 0))
        {
            for (size_t j = 0; j < mpz_get_ui(s); j++)
            {
                square_multiply(y, y, two, n);

                if (mpz_cmp(y, one_mod_n) == 0)
                    return 0;
                if (mpz_cmp(y, min_one_mod_n) == 0)
                    break;
            }
        }
    }

    gmp_randclear(state);
    mpz_clears(s, t, y, a, one, two, min_one, n_min_one, one_mod_n, min_one_mod_n, NULL);

    return 1;
}

void execute_test_miller_rabin(mpz_t n, size_t k, args_opt opt)
{
    double tmps;
    clock_t begin, end;

    if (opt.flag_t)
        begin = clock();

    printf("=> n est %s\n", test_miller_rabin(n, k) ? "premier" : "composé");

    if (opt.flag_t)
    {
        end = clock();
        tmps = end - begin;
        tmps = tmps / CLOCKS_PER_SEC;
        printf("-> Temps: %f sec\n   Itération: %f sec\n", tmps, tmps / k);
    }
}
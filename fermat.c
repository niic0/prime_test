#include "./headers/fermat.h"

/*
 * Fonction: test_fermat
 * ---------------------
 *  Implementation de l'algorithme du test de Fermat. L'algorithme consiste a
 *  tester si pour tout a pris au hasard entre 1 et n-1 (1 < a < n-1) on a bien
 *  a^(n-1) mod n = 1 (1). On boucle k fois pour tester si pour chaque a pris
 *  au hasard, l'equation (1) est respectee. Plus k est grand, plus
 *  l'algorithme est fiable.
 *  Si n est inferieur a 3, l'algorithme ne fonctionne pas car il n'y a pas de
 *  nombre reel strictement superieur a 1 est strictement inferieur a 2.
 *
 *  n: Nombre a tester
 *  k: Nombre de repetitions voulue
 *
 *  retour: Retourne 1 si n est premier, 0 si il est compose. Si le k est trop
 *          petit, le resultat est moins probable et donc moins fiable.
 */
int test_fermat(mpz_t n, size_t k)
{
    mpz_t a, res, n_min_one, n_min_two;
    gmp_randstate_t state;

    mpz_inits(a, res, n_min_one, n_min_two, NULL);

    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_sub_ui(n_min_one, n, 1);
    mpz_sub_ui(n_min_two, n, 2);

    for (size_t i = 0; i < k; i++)
    {
        mpz_urandomm(a, state, n_min_two);
        mpz_add_ui(a, a, 1);
        square_multiply(res, a, n_min_one, n);

        if (mpz_cmp_si(res, 1) != 0) // Si a^(n-1) mod n != 1
            return 0;                // Retourne composee
    }

    mpz_clears(a, res, n_min_one, NULL);
    gmp_randclear(state);

    return 1;
}

void execute_test_fermat(mpz_t n, size_t k, args_opt opt)
{
    double tmps;
    clock_t begin, end;

    if (opt.flag_t)
        begin = clock();

    printf("=> n est %s\n", test_fermat(n, k) ? "premier" : "composé");

    if (opt.flag_t)
    {
        end = clock();
        tmps = end - begin;
        tmps = tmps / CLOCKS_PER_SEC;
        printf("-> Temps: %f sec\n   Itération: %f sec\n", tmps, tmps / k);
    }
}
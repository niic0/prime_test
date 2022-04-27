#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

// Implementation du test de Fermat.
int test_fermat(mpz_t n, size_t k);

// Implementation du test de Miller Rabin.
int test_miller_rabin(mpz_t n, size_t k);

// Permet de faire les tests demandes par l'utilisateur
void prime_test(args_opt opt);

int main(int argc, char **argv)
{
    // options entrees par l'utilisateur
    args_opt opt;

    if (handle_args(argc, argv, &opt))
        return 1;

    prime_test(opt);

    return 0;
}

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

/*
 * Fonction: prime_test
 * --------------------
 *  Fonction permettant de repondre aux demandes (options) de l'utilisateur.
 *
 *  opt: strucure d'options a tester
 *
 *  retour: Aucun retour.
 */
void prime_test(args_opt opt)
{
    mpz_t n;
    size_t k;

    clock_t begin, end;
    double time_fermat = -1;
    double time_miller = -1;

    if (!opt.flag_f && !opt.flag_k && !opt.flag_mr && !opt.flag_n && !opt.flag_t)
        usage();

    // si un k est donne par l'utilisateur
    if (opt.flag_k)
        k = opt.k;
    else
    {
        k = 80;
        printf("k = %ld\n", k);
    }

    // si un n est donne par l'utilisateur
    if (opt.flag_n)
        mpz_init_set(n, opt.n);
    else
    {
        srand(time(NULL));
        mpz_init_set_si(n, rand() % 10000000);
        gmp_printf("n = %Zu\n", n);
    }

    // teste de fermat
    if (opt.flag_f || (!opt.flag_f && !opt.flag_mr))
    {
        if (opt.flag_t)
            begin = clock();

        puts("\n# TESTE DE FERMAT");
        printf("=> n est %s\n", test_fermat(n, k) ? "premier" : "composé");

        if (opt.flag_t)
        {
            end = clock();
            time_fermat = end - begin;
        }
    }

    // teste de miller-rabin
    if (opt.flag_mr || (!opt.flag_f && !opt.flag_mr))
    {
        if (opt.flag_t)
            begin = clock();

        puts("\n# TESTE DE MILLER-RABIN");
        printf("=> n est %s\n", test_miller_rabin(n, k) ? "premier" : "composé");

        if (opt.flag_t)
        {
            end = clock();
            time_miller = end - begin;
        }
    }

    // Affiche le temps
    if (opt.flag_t)
    {
        puts("\n# Temps");
        if (time_fermat != -1)
            printf("- Fermat: %f secondes\n", time_fermat / CLOCKS_PER_SEC);
        if (time_miller != -1)
            printf("- Miller-Rabin: %f secondes\n", time_miller / CLOCKS_PER_SEC);
    }

    // Affiche l'utilisation du programme
    if (opt.flag_h)
        usage();

    mpz_clear(n);
}
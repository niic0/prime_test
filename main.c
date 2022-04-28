#include "./headers/fermat.h"
#include "./headers/miller_rabin.h"
#include "./headers/miller_rabin_multithread.h"
#include "./headers/utils.h"

// Permet de faire les tests demandes par l'utilisateur
void prime_test(args_opt opt);

#define NUM_THREADS 32

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

    if (!opt.flag_f && !opt.flag_k && !opt.flag_mr && !opt.flag_n && !opt.flag_t)
        usage();

    // si un k est donne par l'utilisateur
    if (opt.flag_k)
        k = opt.k;
    else
    {
        k = 80;
        printf("\nk = %ld\n", k);
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

    // teste de miller-rabin multithreads
    if (opt.flag_mr || (!opt.flag_f && !opt.flag_mr))
    {
        puts("\n# TESTE DE MILLER-RABIN MULTITHREADING");
        execute_miller_rabin_multithread(n, k, opt, NUM_THREADS);
    }

    // teste de miller-rabin
    if (opt.flag_mr || (!opt.flag_f && !opt.flag_mr))
    {
        puts("\n# TESTE DE MILLER-RABIN");
        execute_test_miller_rabin(n, k, opt);
    }

    // teste de fermat
    if (opt.flag_f || (!opt.flag_f && !opt.flag_mr))
    {
        puts("\n# TESTE DE FERMAT");
        execute_test_fermat(n, k, opt);
    }


    // Affiche l'utilisation du programme
    if (opt.flag_h)
        usage();

    mpz_clear(n);
}
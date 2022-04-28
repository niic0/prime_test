#include "./headers/utils.h"
#include "./headers/miller_rabin_multithread.h"

// Miller rabin multithreading
void *miller_rabin_multithread(void *args)
{
    n_and_k *thread_args = (n_and_k *)args;

    mpz_t n;
    mpz_init_set(n, thread_args->n);
    size_t k = thread_args->k;

    if (mpz_get_ui(n) % 2 == 0)
    {
        if (mpz_cmp_ui(n, 2) == 0)
            return (void *)1;
        else
            return (void *)0;
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
                    return (void *) 0;
                if (mpz_cmp(y, min_one_mod_n) == 0)
                    break;
            }
        }
    }

    gmp_randclear(state);
    mpz_clears(s, t, y, a, one, two, min_one, n_min_one, one_mod_n, min_one_mod_n, NULL);

    return (void*) 1;
    pthread_exit(NULL);
}


int execute_miller_rabin_multithread(mpz_t n, size_t k, args_opt opt, size_t NUM_THREADS)
{
    printf("--> THREADS: %ld\n", NUM_THREADS);

    struct timespec start, finish;
    double elapsed;
    int rc;
    int premier = 0;

    void *status;
    pthread_t threads[NUM_THREADS];
    n_and_k thread_args;

    mpz_init_set(thread_args.n, opt.n);
    thread_args.k = k / NUM_THREADS;

    if (opt.flag_t)
        clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_create(&threads[i], NULL, miller_rabin_multithread, (void *)&thread_args);

        if (rc)
        {
            printf("Erreur: Impossible de créer un thread\n");
            exit(-1);
        }
    }

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], &status);
        if ((int *)status == (int *)1)
            premier = 1;
    }

    printf("=> n est %s\n", premier ? "premier" : "composé");

    if (opt.flag_t)
    {
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("-> Temps: %f sec\n   Itération: %f sec\n", elapsed, elapsed / k);
    }
    return 0;
}
#include "./headers/utils.h"

/*
 * Fonction: square_multiply
 * -------------------------
 *  Implementation de l'algorithme Square and Multiply appliquee au cas de
 *  l'exponentiation modulaire ou r = a^h mod n.
 *
 *  r: Stocke le resultat de a^h mod n
 *  a: Nombre mis a la puissance h
 *  n: Le modulo
 *  h: Puissance appliquee a a
 * 
 *  retour: Ne retourne rien. Le résultat est stocke dans la variable r.
 */
void square_multiply(mpz_t r, mpz_t a, mpz_t h, mpz_t n)
{
    char *h_bin = malloc(100000 * sizeof(char));

    h_bin = mpz_get_str(NULL, 2, h);
    mpz_set(r, a);

    for (size_t i = 1; i < strlen(h_bin); i++)
    {
        //gmp_printf("n = %Zd , r = %Zd\n", n, r);
        mpz_mul(r, r, r);
        mpz_mod(r, r, n);

        if (h_bin[i] == '1')
        {
            mpz_mul(r, r, a);
            mpz_mod(r, r, n);
        }
    }

    free(h_bin);
}


/*
 * Fonction: decompose
 * -------------------
 *  Decompose n en 2^s * t.
 * 
 *  n: Valeur a decomposer
 *  s: Exposant de la decomposition
 *  t: Multiplication de la decomposition
 * 
 *  retour: Ne retourne rien. Les valeurs trouvees sont stockees dans s et t.
 */
void decompose(mpz_t n,mpz_t s,mpz_t t) {
     mpz_set_ui(s,mpz_scan1(n, 0));

     mpz_tdiv_q_2exp(t, n, mpz_get_ui(s));
     
}

/*
 * Fonction: handle_args
 * ---------------------
 *  Fonction permettant de gerer les options entree par l'utilisateur.
 * 
 *  argc: Nombres d'arguments
 *  argv: Liste des arguments sous forme de chaines de caracteres
 *  opt: structure contenant les fonctions a lancer.
 * 
 *  retour: Retourne 0 si la fonction n'a rencontre aucune erreur,
 *          retourne 1 si l'entree de l'utilisateur n'est pas bonne
 */
int handle_args(int argc, char **argv, args_opt* opt)
{
    opt->flag_f = 0;
    opt->flag_t = 0;
    opt->flag_k = 0;
    opt->flag_mr = 0;
    opt->flag_n = 0;
    opt->flag_h = 0;

    int c;

    while ((c = getopt (argc, argv, "n:k:mfht")) != -1)
    {
        switch (c)
        {
        case 'n':
            opt->flag_n = 1;
            mpz_init_set_str(opt->n, optarg, 10);
            break;

        case 'k':
            opt->flag_k = 1;
            opt->k = atol(optarg);
            printf("handle args, k = %ld\n", opt->k);
            break;

        case 'm':
            opt->flag_mr = 1;
            break;

        case 'f':
            opt->flag_f = 1;
            break;

        case 't':
            opt->flag_t = 1;
            break;

        case 'h':
            opt->flag_h = 1;
            break;

        default:
            printf("[ERREUR] : Les options sont invalides.\n");
            usage();
            return 1;
        }
    }

    return 0;
}


/*
 * Fonction: usage
 * ---------------
 *  Affiche l'utilsiation du programme en cas d'erreur ou si l'utilisateur
 *  le demande.
 */
void usage() {
    printf("\n[USAGE]\n"
           "./prime_test -[OPTIONS]\n"
           "Options:\n"
           "  -n x : Impose un n pour le teste ou les testes demandé(s). Exemple: ./prime_test -n 123456789\n"
           "  -k x : Fait faire x itérations pour le teste ou les testes demandé(s). Exemple: ./prime_test -k 50\n"
           "  -m : teste le nombre avec le teste de Miller Rabin. Exemple: ./prime_test -m\n"
           "  -f : teste le nombre avec le teste de Fermat. Exemple: ./prime_test -f\n"
           "  -t : Donne le temps mis par les tests. Exemple: ./prime_test -n 123456789\n"
           "\nExemple de commande complète:\n ./prime_test -k 70 -n 1114111 -t\n\n");
}
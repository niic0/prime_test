#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <gmp.h>
#include <time.h>

#include "utils.h"


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
    char *h_bin = malloc(1000000 * sizeof(char));

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
 * Fonction: range_rand
 * --------------------
 *  Prend un nombre au hasard dans un ensemble tel que lower < r < upper. Pour
 *  obtenir le nombre aleatoire, on utilise la formule:
 *  NombreAleatoire mod (upper - lower +1) + lower
 *
 *  r: Stocke le nombre aleatoire
 *  lower: Borne basse de l'ensemble
 *  upper: Borne superieur de l'ensemble
 *
 *  retour: Ne retourne rien. Le nombre aleatoire est stocke dans la variable r.
 */
void range_rand(mpz_t r, mpz_t lower, mpz_t upper)
{
    mpz_t one, mod;
    mpz_init(mod);
    mpz_init_set_si(one, 1);

    // lower+1 <= r <= upper-1
    mpz_add(lower, lower, one);
    mpz_sub(upper, upper, one);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_urandomm(r, state, upper);

    // Calcule: mod = upper - lower + 1
    mpz_sub(mod, upper, lower);
    mpz_add(mod, mod, one);

    // Calcule: numberRand % mod + lower;
    mpz_mod(r, r, mod);
    mpz_add(r, r, lower);

    // On remet lower et upper a leur valeur d origine
    mpz_sub(lower, lower, one);
    mpz_add(upper, upper, one);

    mpz_clears(one, mod, NULL);
    gmp_randclear(state);
}


/*
 * Fonction: decompose
 * -------------------
 *  Decompose n en 2^s * t. La variable s est obtenue en comptant le nombre de 
 *  0 apres le bit 1 de poids faible. La variable t est obtenue en tronquant
 *  les bits a 0 apres le bit a 1 de poids faible.
 *  Par exemple: 14 donne 1110 en binaire.
 *    - s = 1 car il y a 1 zero apres le bit de poids faible
 *    - t = 111 en binaire soit 7 en decimal
 *    On obtient bien 14 = 2^1 * 7
 *  
 *  Une fonction est donnee par gmp, mpz_get_d_2exp, mais j'ai prefere faire 
 *  cette fonction moi meme.
 * 
 *  n: Valeur a decomposer
 *  s: Exposant de la decomposition
 *  t: Multiplication de la decomposition
 * 
 *  retour: Ne retourne rien. Les valeurs trouvees sont stockees dans s et t.
 */
void decompose(mpz_t n, mpz_t s, mpz_t t)
{
    mpz_t one;
    mpz_init_set_si(one, 1);

    char* n_bin; // n en binaire
    n_bin = malloc(100000 * sizeof(char));
    mpz_get_str(n_bin, 2, n);

    int i;
    mpz_set_si(s, 0);

    // On lit les bits de droite a gauche pour compter le nombre de 0 (=s)
    for (i = strlen(n_bin) - 1; i >= 0; i--)
    {
        if (n_bin[i] == '1')
            break;
        mpz_add(s, s, one);
    }

    char* t_bin; // t en binaire
    t_bin = malloc(100000 * sizeof(char));

    for (int k = 0; k<=i; k++)
        t_bin[k] = n_bin[k];

    mpz_set_str(t, t_bin, 2);

    mpz_clear(one);
    free(n_bin);
    free(t_bin);
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
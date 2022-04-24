# Teste de Fermat et de Miller Rabin
Implémentation en C des testes de primalités de Fermat et Miller-Rabin avec la bibliothéque GMP et l'algorithme *Square and Multiply*.

## Utilisation
Une exemple du programme est lancé en tapant la commande `make`. L'utilisation du programme est affiché si la commande est lancé seule, c'est à dire en faisant `./prime_test`.

### Commandes
Le programme ce lance avec la commande `./prime_test n k -[OPTIONS]`. *n* et *k* correspondent au nombre a tester et au nombre d'itérations voulue respectivement. Plus *k* est grand, plus le test est fiable. Si aucun *k* n'est donnée, *k* vaut 80 (pour un résultat relativement sûr mais un temps de traitement plus long).

Les options sont les suivantes:
- `-m` : teste le nombre avec le teste de Miller Rabin. Exemple: `./prime_test 1033 -m`
- `-f` : teste le nombre avec le teste de Fermat. Exemple: `./prime_test 5737 40 -f`
- `-k x` : Fait faire *x* itérations pour le teste ou les testes demandé(s). Exemple: `./prime_test -k 50`
- `-n x` : Impose un n pour le teste ou les testes demandé(s). Exemple: `./prime_test -n 123456789`
- `-t` : Donne le temps mis par les tests. Exemple: `./prime_test -n 123456789`


* Si aucun *n* n'est demandé, 10 nombres seront pris au hasard entre 100 et 10.000.000 puis testés. Si vous souhaitez tout de même donner un nombre d'itération *k*, vous devez utiliser la commande `-k x`(où *x* est le nombre d'itérations voulue).
* Si aucun teste n'est demandé (ni `-m` ni `-f`) alors les deux testes seront lancés est l'affichage montrera le résultat des deux algorithmes.

## Fichier source et informations sur les fonctions
Les testes de primalités sont dans le fichier `main.c`. les fonctions "outils" comme *square and multiply* ou la fonciton de décomposition sont dans `utils.c`. Le makefile génére les fichiers objects `.o` `main.o` et `utils.o` nécessairent pour la création de l'executable `test_prime`.

Le fonctionnement complet de chaque fonction est noté en commentaire au dessus de celle-ci dans les fichier `.c`. Un bref descriptif des fonctions est noté en commentaire au dessus de celles-ci dans les fichiers headers `.h`.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Position {
    int numero_ligne;
    int ordre;
    int numero_phrase;
    struct Position* suivant;
} t_Position;

typedef struct listePositions {
    t_Position* debut;
    int nb_elements;
} t_listePositions;


typedef struct Noeud {
    char *mot;
    int nb_occurences;
    t_listePositions* positions;
    struct Noeud* filsGauche;
    struct Noeud* filsDroit;
} t_Noeud;

typedef struct listePhrase {
    char* Phrase;
    int id_phrase;
    struct listePhrase* suivant;
} t_Phrase;

typedef struct t_index {
    t_Noeud * racine;
    int nb_mots_differents;
    int nb_mots_total;
    struct listePhrase * debut_phrase;
}t_Index;

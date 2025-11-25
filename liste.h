#ifndef LISTE_H
#define LISTE_H

#include "truc.h"
#include "ligne.h"
#include "abr_type.h"

typedef struct _un_elem {
    Un_truc *truc; //Une station ou une connexion
    struct _un_elem *suiv;
} Un_elem;

Un_elem *inserer_liste_trie(Un_elem *liste, Un_truc *truc);
void ecrire_liste( FILE *flux, Un_elem *liste);
void detruire_liste(Un_elem *liste);
void detruire_liste_et_truc(Un_elem *liste);
Un_elem *lire_stations( char *nom_fichier);
void limites_zone(Un_elem *liste, Une_coord *limite_no, Une_coord *limite_se);
Un_elem *inserer_deb_liste(Un_elem *liste, Un_truc *truc);
Un_elem *lire_connexions(char *nom_fichier, Une_ligne *liste_ligne, Un_nabr *abr_sta); 
Un_truc *extraire_deb_liste(Un_elem **liste);
Un_truc *extraire_liste(Un_elem **liste, Un_truc *truc);

/* Fonctions ajoutées par nous même */

/**
 * permet d'ajouter "b" dans le tableau des connexion de "a"
 */
void ajout_connexion(Un_truc *a, Un_truc *b);

#endif
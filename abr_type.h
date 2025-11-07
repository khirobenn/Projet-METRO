#ifndef ABR_TYPE_H
#define ABR_TYPE_H

#include "truc.h"

typedef struct _un_elem Un_elem;

typedef struct _un_nabr {
    Un_truc *truc; //La station
    struct _un_nabr *g; //Fils gauche strictement inferieur
    struct _un_nabr *d; //Fils droit
} Un_nabr;

Un_nabr *creer_nabr(Un_truc *truc);
Un_nabr *inserer_abr(Un_nabr *abr, Un_nabr *n);
Un_nabr *construire_abr(Un_elem *liste_sta);
void detruire_abr(Un_nabr *abr);
Un_truc *chercher_station(Un_nabr *abr, char *nom);
void print_arb(Un_nabr *a);

#endif
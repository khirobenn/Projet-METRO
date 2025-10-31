#include <stdio.h>
#include "abr_type.h"
#include <string.h>

Un_nabr *creer_nabr(Un_truc *truc){
    Un_nabr *abr = malloc(sizeof(Un_nabr));
    abr->truc = truc;
    abr->d = NULL;
    abr->g = NULL;
    return abr;
}

Un_nabr *inserer_abr(Un_nabr *abr, Un_nabr *n){
    if(n->truc->type == CON) return abr;
    if(abr == NULL) return n;

    int cmp = strcmp(n->truc->data.sta.nom, abr->truc->data.sta.nom);

    if(cmp >= 0) abr->d = inserer_abr(abr->d, n);
    else abr->g = inserer_abr(abr->g, n);

    return abr;
}

Un_nabr *construire_abr(Un_elem *liste_sta){
    Un_nabr *abr = NULL;

    while(liste_sta != NULL){
        if(liste_sta->truc->type == STA){
            Un_nabr *n = creer_nabr(liste_sta->truc);
            abr = inserer_abr(abr, n);
        }
    }

    return abr;
}

void detruire_abr(Un_nabr *abr){
    if(abr == NULL) return;
    Un_nabr *d = abr->d;
    Un_nabr *g = abr->g;

    abr->d = NULL;
    abr->g = NULL;
    abr->truc = NULL;
    free(abr);

    detruire_abr(d);
    detruire_abr(g);
}

Un_truc *chercher_station(Un_nabr *abr, char *nom){
    if(abr == NULL) return NULL;
    int cmp = strcmp(nom, abr->truc->data.sta.nom);
    if(cmp == 0) return abr->truc;
    else if(cmp < 0) return chercher_station(abr->g, nom);
    return chercher_station(abr->d, nom);
}
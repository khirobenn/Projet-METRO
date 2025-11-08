#include <stdio.h>
#include <stdlib.h>
#include "aqrtopo.h"

Un_noeud *inserer_aqr(Un_noeud *aqr, Une_coord limite_no, Une_coord limite_se, Un_truc *truc){
    if(aqr == NULL){
        Un_noeud *n = malloc(sizeof(Un_noeud));
        n->limite_no = limite_no;
        n->limite_se = limite_se;
        n->ne = NULL;
        n->no = NULL;
        n->se = NULL;
        n->so = NULL;
        n->truc = truc;
        return n;
    }
    if(truc->coord.lat > aqr->truc->coord.lat && truc->coord.lon > aqr->truc->coord.lon){
        aqr->ne = inserer_aqr(aqr->ne, limite_no, limite_se, truc);
    }
    else if(truc->coord.lat > aqr->truc->coord.lat && truc->coord.lon < aqr->truc->coord.lon){
        aqr->se = inserer_aqr(aqr->se, limite_no, limite_se, truc);
    }
    else if(truc->coord.lat < aqr->truc->coord.lat && truc->coord.lon > aqr->truc->coord.lon){
        aqr->no = inserer_aqr(aqr->no, limite_no, limite_se, truc);
    }
    else if(truc->coord.lat < aqr->truc->coord.lat && truc->coord.lon < aqr->truc->coord.lon){
        aqr->so = inserer_aqr(aqr->so, limite_no, limite_se, truc);
    }
    else{
        // à écrire cas ou c'est égal
    }
    return aqr;
}


Un_noeud *construire_aqr(Un_elem *liste){
    Une_coord limite_no, limite_se;
    limites_zone(liste, &limite_no, &limite_se);
    Un_noeud *n = NULL;
    while(liste != NULL){
        inserer_aqr(n, limite_no, limite_se, liste->truc);
        liste = liste->suiv;
    }
    return n;
}

void detruire_aqr(Un_noeud *aqr){
    if(aqr != NULL){
        detruire_aqr(aqr->ne);
        detruire_aqr(aqr->no);
        detruire_aqr(aqr->se);
        detruire_aqr(aqr->so);

        aqr->ne = NULL;
        aqr->no = NULL;
        aqr->se = NULL;
        aqr->so = NULL;
        aqr->truc = NULL;
        free(aqr);
    }
}

Un_truc *chercher_aqr(Un_noeud *aqr, Une_coord coord){
    if(aqr == NULL) return NULL;
    if(coord.lat > aqr->truc->coord.lat && coord.lon > aqr->truc->coord.lon){
        return chercher_aqr(aqr->ne, coord);
    }
    else if(coord.lat > aqr->truc->coord.lat && coord.lon < aqr->truc->coord.lon){
        return chercher_aqr(aqr->se, coord);
    }
    else if(coord.lat < aqr->truc->coord.lat && coord.lon > aqr->truc->coord.lon){
        return chercher_aqr(aqr->no, coord);
    }
    else if(coord.lat < aqr->truc->coord.lat && coord.lon < aqr->truc->coord.lon){
        return chercher_aqr(aqr->so, coord);
    }
    // càd les coordonnées sont égales
    return aqr->truc;
}

Un_elem *chercher_zone(Un_noeud *aqr, Un_elem *liste, Une_coord limite_no, Une_coord limite_se){
    // à définir
}
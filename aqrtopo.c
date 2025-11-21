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

    Une_coord limite_mil = {.lat = (limite_no.lat + limite_se.lat)/2, .lon = (limite_no.lon + limite_se.lon)/2};

    if(truc->coord.lat > limite_mil.lat && truc->coord.lon < limite_mil.lon){
        aqr->se = inserer_aqr(aqr->se, limite_no, limite_mil, truc);
    }
    else if(truc->coord.lat < limite_mil.lat && truc->coord.lon > limite_mil.lon){
        aqr->no = inserer_aqr(aqr->no, limite_mil, limite_se, truc);
    }
    else if(truc->coord.lat > limite_mil.lat && truc->coord.lon > limite_mil.lon){
        Une_coord c1 = {.lat = limite_no.lat, .lon = limite_mil.lon};
        Une_coord c2 = {.lat = limite_mil.lat, .lon = limite_se.lon};
        aqr->ne = inserer_aqr(aqr->ne, c1, c2, truc);
    }
    else if(truc->coord.lat < limite_mil.lat && truc->coord.lon < limite_mil.lon){
        Une_coord c1 = {.lat = limite_mil.lat, .lon = limite_no.lon};
        Une_coord c2 = {.lat = limite_se.lat, .lon = limite_mil.lon};
        aqr->so = inserer_aqr(aqr->so, c1, c2, truc);
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
        n = inserer_aqr(n, limite_no, limite_se, liste->truc);
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
    if(aqr->truc->coord.lat == coord.lat && aqr->truc->coord.lon == coord.lon) return aqr-> truc;
    
    Une_coord limite_mil = {.lat = (aqr->limite_no.lat + aqr->limite_se.lat)/2, .lon = (aqr->limite_no.lon + aqr->limite_se.lon)/2};
    if(coord.lat > limite_mil.lat && coord.lon < limite_mil.lon){
        return chercher_aqr(aqr->se, coord);
    }
    else if(coord.lat < limite_mil.lat && coord.lon > limite_mil.lon){
        return chercher_aqr(aqr->no, coord);
    }
    else if(coord.lat > limite_mil.lat && coord.lon > limite_mil.lon){
        return chercher_aqr(aqr->ne, coord);
    }
    return chercher_aqr(aqr->so, coord);
}

Un_elem *chercher_zone(Un_noeud *aqr, Un_elem *liste, Une_coord limite_no, Une_coord limite_se){
    // à définir
}
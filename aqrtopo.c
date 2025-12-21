#include <stdio.h>
#include <stdlib.h>
#include "aqrtopo.h"
#include <string.h>

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
    Une_coord limite_mil = {.lat = (limite_se.lat + limite_no.lat)/2, .lon = (limite_se.lon + limite_no.lon)/2};

    if(truc->coord.lat >= limite_mil.lat && truc->coord.lon <= limite_mil.lon){
        aqr->se = inserer_aqr(aqr->se, limite_mil, limite_se, truc);
    }
    else if(truc->coord.lat <= limite_mil.lat && truc->coord.lon >= limite_mil.lon){
        aqr->no = inserer_aqr(aqr->no, limite_no, limite_mil, truc);
    }
    else if(truc->coord.lat >= limite_mil.lat && truc->coord.lon >= limite_mil.lon){
        Une_coord sud_est = {.lat = limite_se.lat, .lon = limite_mil.lon};
        Une_coord nord_ouest = {.lat = limite_mil.lat, .lon = limite_no.lon};
        aqr->ne = inserer_aqr(aqr->ne, nord_ouest, sud_est, truc);
    }
    else if(truc->coord.lat <= limite_mil.lat && truc->coord.lon <= limite_mil.lon){
        Une_coord sud_est = {.lat = limite_mil.lat, .lon = limite_se.lon};
        Une_coord nord_ouest = {.lat = limite_no.lat, .lon = limite_mil.lon};
        aqr->so = inserer_aqr(aqr->so, nord_ouest, sud_est, truc);
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
    if(aqr->truc->coord.lat == coord.lat && aqr->truc->coord.lon == coord.lon) return aqr->truc;
    Une_coord limite_mil = {.lat = (aqr->limite_se.lat + aqr->limite_no.lat)/2, .lon = (aqr->limite_se.lon + aqr->limite_no.lon)/2};
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

int point_est_dans_carre(Une_coord point, Une_coord x, Une_coord y){
    if(point.lat <= y.lat && point.lat >= x.lat && point.lon <= x.lon && point.lon >= y.lon){
        return 1;
    }
    return 0;
}

// Si le carré 1 est dans le carré 2
int chevauche_carres(Une_coord x1, Une_coord y1,Une_coord x2, Une_coord y2){
    Une_coord z1 = {.lat = x1.lat, .lon = y1.lon};
    Une_coord d1 = {.lat = y1.lat, .lon = x1.lon};
    Une_coord z2 = {.lat = x2.lat, .lon = y2.lon};
    Une_coord d2 = {.lat = y2.lat, .lon = x2.lon};
    if(point_est_dans_carre(x1, x2, y2) || point_est_dans_carre(y1, x2, y2) || point_est_dans_carre(z1, x2, y2) || point_est_dans_carre(d1, x2, y2) || point_est_dans_carre(x2, x1, y1) || point_est_dans_carre(y2, x1, y1) || point_est_dans_carre(z2, x1, y1) || point_est_dans_carre(d2, x1, y1)){
        return 1;
    }
    return 0;
}

Un_elem *chercher_zone(Un_noeud *aqr, Un_elem *liste, Une_coord limite_se, Une_coord limite_no){
    if (aqr == NULL) return liste;
    if (point_est_dans_carre(aqr->truc->coord, limite_no, limite_se)){
        liste = inserer_liste_trie(liste, aqr->truc);
    }
    Une_coord limite_mil = {.lat = (aqr->limite_se.lat + aqr->limite_no.lat)/2, .lon = (aqr->limite_se.lon + aqr->limite_no.lon)/2};
    
    // Coordonnées du carré au Nord Est
    Une_coord nord_est_no = {.lat = limite_mil.lat, .lon = aqr->limite_no.lon};
    Une_coord nord_est_se = {.lat = aqr->limite_se.lat, .lon = limite_mil.lon};

    // Coordonnées du carré au Sud Ouest
    Une_coord sud_ouest_no = {.lat = aqr->limite_no.lat, .lon = limite_mil.lon};
    Une_coord sud_ouest_se = {.lat = limite_mil.lat, .lon = aqr->limite_se.lon};

    // printf("no : %lf, %lf, se : %lf, %lf\n", aqr->limite_no.lat, aqr->limite_no.lon, aqr->limite_se.lat, aqr->limite_se.lon);
    // printf("chauvauce : %d\n", point_est_dans_carre(aqr->limite_no, limite_no, limite_se));

    if(chevauche_carres(aqr->limite_no, limite_mil, limite_no, limite_se)){
        liste = chercher_zone(aqr->no, liste, limite_se, limite_no);
    }

    if(chevauche_carres(limite_mil, aqr->limite_se, limite_no, limite_se)){
        liste = chercher_zone(aqr->se, liste, limite_se, limite_no);
    }

    if(chevauche_carres(nord_est_no, nord_est_se, limite_no, limite_se)){
        liste = chercher_zone(aqr->ne, liste, limite_se, limite_no);
    }

    if(chevauche_carres(sud_ouest_no, sud_ouest_se, limite_no, limite_se)){
        liste = chercher_zone(aqr->so, liste, limite_se, limite_no);
    }
    return liste;
}

void print_aqr(Un_noeud *aqr){
    if(aqr != NULL){
        printf("%s\n", aqr->truc->data.sta.nom);
        print_aqr(aqr->ne);
        print_aqr(aqr->se);
        print_aqr(aqr->so);
        print_aqr(aqr->no);
    }
}
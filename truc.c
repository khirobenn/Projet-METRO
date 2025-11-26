#include <stdio.h>
#include <stdlib.h>
#include "truc.h"

#define LARGEUR 1200
#define LONGUEUR 800

Un_truc *creer_truc(Une_coord coord, Ttype type, Tdata data, double uv){
    Un_truc *truc = (Un_truc *) malloc(sizeof(Un_truc));
    truc->coord = coord; 
    truc->type = type; 
    truc->data = data;
    truc->user_val = uv;
    truc->data.sta.point = sfCircleShape_create();
    sfCircleShape_setRadius(truc->data.sta.point, 8);
    sfCircleShape_setFillColor(truc->data.sta.point, sfBlue);
    return truc;
}

void detruire_truc(Un_truc *truc){
    if(truc->type == STA){
        free(truc->data.sta.nom);
        sfCircleShape_destroy(truc->data.sta.point);
        if(truc->data.sta.tab_con != NULL) free(truc->data.sta.tab_con);
    }
    free(truc);
}
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
    sfCircleShape_setRadius(truc->data.sta.point, 4);
    sfCircleShape_setFillColor(truc->data.sta.point, sfBlue);
    truc->data.sta.nom_shape = sfText_create();
    sfText_setString(truc->data.sta.nom_shape, truc->data.sta.nom);
    sfText_setFillColor(truc->data.sta.nom_shape, sfBlack);
    return truc;
}

void detruire_truc(Un_truc *truc){
    if(truc->type == STA){
        free(truc->data.sta.nom);
        sfCircleShape_destroy(truc->data.sta.point);
        sfText_destroy(truc->data.sta.nom_shape);
        if(truc->data.sta.tab_con != NULL) free(truc->data.sta.tab_con);
    }
    free(truc);
}
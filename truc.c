#include <stdio.h>
#include <stdlib.h>
#include "truc.h"

Un_truc *creer_truc(Une_coord coord, Ttype type, Tdata data, double uv){
    Un_truc *truc = (Un_truc *) malloc(sizeof(Un_truc));
    truc->coord = coord; 
    truc->type = type; 
    truc->data = data;
    truc->user_val = uv;
    return truc;
}

void detruire_truc(Un_truc *truc){
    if(truc->type == STA){
        free(truc->data.sta.nom);
        if(truc->data.sta.tab_con != NULL) free(truc->data.sta.tab_con);
    }
    free(truc);
}
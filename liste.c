#include <stdio.h>
#include <stdlib.h>
#include "liste.h"
#include <string.h>
#include <float.h>

Un_elem *inserer_liste_trie(Un_elem *liste, Un_truc *truc){
    if(liste == NULL){
        Un_elem *l = malloc(sizeof(Un_elem));
        l->truc = truc;
        l->suiv = NULL;
        return l;
    }
    
    if(truc->user_val < liste->truc->user_val){
        Un_elem *l = malloc(sizeof(Un_elem));
        l->truc = truc;
        l->suiv = liste;
        return l;
    }
    
    liste->suiv = inserer_liste_trie(liste->suiv, truc);
    return liste;
}

void ecrire_liste( FILE *flux, Un_elem *liste){
    while(liste != NULL){
        if(liste->truc->type == STA){
            printf("Latitude : %f\tLongitude: %f\t nom : %s\n", liste->truc->coord.lat, liste->truc->coord.lon, liste->truc->data.sta.nom);
        }
        liste = liste->suiv;
    }
}

void detruire_liste(Un_elem *liste){
    Un_elem *tmp = liste;
    while(tmp != NULL){
        liste = liste->suiv;
        tmp->suiv = NULL;
        free(tmp);
        tmp = liste;
    }
}

void detruire_liste_et_truc(Un_elem *liste){
    Un_elem *tmp = liste;
    while(tmp != NULL){
        liste = liste->suiv;
        tmp->suiv = NULL;
        detruire_truc(tmp->truc);
        free(tmp);
        tmp = liste;
    }
}

Un_elem *lire_stations( char *nom_fichier){
    FILE *f = fopen(nom_fichier, "r");
    int a = 0;
    float lat;
    float lon;
    char nom_lecture[20];
    Un_elem * head = NULL;
    // %[^\n]s trouvé sur stack overflow pour lire une chaine de caractères avec espace
    while(fscanf(f, "%f;%f;%[^\n]s\n", &lon, &lat, nom_lecture) != EOF){
        char *nom = malloc(sizeof(20));
        strcpy(nom, nom_lecture);
        Une_coord cord = {.lon = lon, .lat = lat};

        Une_station station = {.nom = nom, .tab_con = NULL, .con_pcc = NULL, .nb_con = 0};

        Tdata data;
        data.sta = station;
        
        Un_truc *truc = creer_truc(cord, STA, data, 0);

        head = inserer_liste_trie(head, truc);
    }
    fclose(f);
    return head;
}

void limites_zone(Un_elem *liste, Une_coord *limite_no, Une_coord *limite_se){
    if(liste == NULL) return;
    limite_no->lon = FLT_MAX;
    limite_no->lat = FLT_MIN;

    limite_se->lon = FLT_MIN;
    limite_se->lat = FLT_MAX;

    while(liste != NULL){
        if(liste->truc->coord.lon < limite_no->lon) limite_no->lon = liste->truc->coord.lon;
        if(liste->truc->coord.lat > limite_no->lat) limite_no->lat = liste->truc->coord.lat;

        if(liste->truc->coord.lon > limite_se->lon) limite_se->lon = liste->truc->coord.lon;
        if(liste->truc->coord.lat < limite_se->lat) limite_se->lat = liste->truc->coord.lat;

        liste = liste->suiv;
    }
}
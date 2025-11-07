#include <stdio.h>
#include <stdlib.h>
#include "liste.h"
#include <string.h>
#include <float.h>

#define TAILLE_NOM 40

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
        else{
            printf("Pour aller de %s à %s, prenez la ligne %s avec une durée de %f minutes.\n", liste->truc->data.con.sta_dep->data.sta.nom, liste->truc->data.con.sta_arr->data.sta.nom, liste->truc->data.con.ligne->code, liste->truc->user_val);
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
    float lat;
    float lon;
    char nom_lecture[TAILLE_NOM];
    Un_elem * head = NULL;
    // %[^\n]s trouvé sur stack overflow pour lire une chaine de caractères avec espace
    while(fscanf(f, "%f;%f;%[^\n]\n", &lon, &lat, nom_lecture) != EOF){
        char *nom = malloc(sizeof(char)*TAILLE_NOM);
        strcpy(nom, nom_lecture);
        Une_coord cord = {.lon = lon, .lat = lat};

        Une_station station = {.nom = nom, .tab_con = NULL, .tab_con = NULL, .nb_con = 0};

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

Un_elem *inserer_deb_liste(Un_elem *liste, Un_truc *truc){
    Un_elem *l = malloc(sizeof(Un_elem));
    l->truc = truc;
    l->suiv = liste;
    return l;
}

Un_elem *lire_connexions(char *nom_fichier, Une_ligne *liste_ligne, Un_nabr *abr_sta){
    FILE *f = fopen(nom_fichier, "r");
    if(f == NULL){
        printf("Erreur lecture du fichier!\n");
        return NULL;
    }

    char line[100];
    Un_elem *connexion = NULL;

    while(fgets(line, 100, f) != NULL){
        printf("%s", line);
        if(line[0] != '#'){
            char code_ligne[CODE_TAILLE];
            char nom_sta1[TAILLE_NOM];
            char nom_sta2[TAILLE_NOM];
            float duree;
            sscanf(line, "%s ; %[^;] ; %[^;] ; %f\n", code_ligne, nom_sta1, nom_sta2, &duree);
            nom_sta1[strlen(nom_sta1) - 1] = '\0';
            nom_sta2[strlen(nom_sta2) - 1] = '\0';
            Une_ligne * ligne = chercher_ligne(liste_ligne, code_ligne);
            if(ligne != NULL){
                Un_truc *sta1 = chercher_station(abr_sta, nom_sta1);
                Un_truc *sta2 = chercher_station(abr_sta, nom_sta2);

                // %%Calcul de la durée si duree == 0
                if(sta1 != NULL && sta2 != NULL){
                    ajout_connexion(sta1, sta2);
                    ajout_connexion(sta2, sta1);
                    printf("> Connexion ajouté avec succés.\n\n");
                }
                else{
                    printf("> Erreur : station non existante.\n\n");
                }

                Tdata data = {.con.ligne = ligne, .con.sta_dep = sta1, .con.sta_arr = sta2};
                Une_coord cord = {.lat = .0, .lon = .0};

                Un_truc *truc = creer_truc(cord, CON, data, duree);
                connexion = inserer_deb_liste(connexion, truc);
            }
        }
    }
    fclose(f);
    return connexion;
}

void ajout_connexion(Un_truc *a, Un_truc *b){
    if(a->data.sta.nb_con == 0){
        a->data.sta.tab_con = malloc(sizeof(Un_truc *));
        if(a->data.sta.tab_con == NULL){
            printf("Erreur allocation tableau de connexions\n");
            return;
        }
    }
    else{
        a->data.sta.tab_con = realloc(a->data.sta.tab_con, sizeof(Un_truc *) * (a->data.sta.nb_con + 1));
        if(a->data.sta.tab_con == NULL){
            printf("Erreur reallocation tableau de connexions\n");
            return;
        }
    }
    a->data.sta.tab_con[a->data.sta.nb_con] = b;
    (a->data.sta.nb_con)++;
}
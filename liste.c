#include <stdio.h>
#include <stdlib.h>
#include "liste.h"
#include <string.h>
#include <float.h>

#define TAILLE_NOM 128

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
            printf("Latitude : %f\tLongitude: %.2f\t nom : %s", liste->truc->coord.lat, liste->truc->coord.lon, liste->truc->data.sta.nom);
            printf("\33[1E");
        }
        else{
            printf("> %s : %s ---> %s | temps estimation :  %.2f minutes.", liste->truc->data.con.ligne->code, liste->truc->data.con.sta_dep->data.sta.nom, liste->truc->data.con.sta_arr->data.sta.nom, liste->truc->user_val);
            printf("\33[1E");
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
        
        Un_truc *truc = creer_truc(cord, STA, data, cord.lat);

        head = inserer_liste_trie(head, truc);
    }
    fclose(f);
    return head;
}

void limites_zone(Un_elem *liste, Une_coord *nord_ouest, Une_coord *sud_est){
    if(liste == NULL) return;
    sud_est->lon = FLT_MAX;
    sud_est->lat = FLT_MIN;

    nord_ouest->lon = FLT_MIN;
    nord_ouest->lat = FLT_MAX;

    while(liste != NULL){
        if(liste->truc->coord.lon < sud_est->lon) sud_est->lon = liste->truc->coord.lon;
        if(liste->truc->coord.lat > sud_est->lat) sud_est->lat = liste->truc->coord.lat;

        if(liste->truc->coord.lon > nord_ouest->lon) nord_ouest->lon = liste->truc->coord.lon;
        if(liste->truc->coord.lat < nord_ouest->lat) nord_ouest->lat = liste->truc->coord.lat;

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
        return NULL;
    }

    char line[200];
    Un_elem *connexion = NULL;

    while(fgets(line, 200, f) != NULL){
        if(line[0] != '#'){
            char code_ligne[CODE_TAILLE];
            char nom_sta1[TAILLE_NOM];
            char nom_sta2[TAILLE_NOM];
            float duree;
            sscanf(line, "%s ; %[^;] ; %[^;] ; %f", code_ligne, nom_sta1, nom_sta2, &duree);
            nom_sta1[strlen(nom_sta1) - 1] = '\0';
            nom_sta2[strlen(nom_sta2) - 1] = '\0';
            Une_ligne * ligne = chercher_ligne(liste_ligne, code_ligne);
            if(ligne != NULL){
                Un_truc *sta1 = chercher_station(abr_sta, nom_sta1);
                Un_truc *sta2 = chercher_station(abr_sta, nom_sta2);

                // %%Calcul de la durée si duree == 0
                if(sta1 != NULL && sta2 != NULL){
                    Tdata data = {.con.ligne = ligne, .con.sta_dep = sta1, .con.sta_arr = sta2};
                    Une_coord cord = {.lat = .0, .lon = .0};
                    
                    Un_truc *truc = creer_truc(cord, CON, data, duree);
                    ajout_connexion(sta1, truc);
                    truc->data.con.ligne_dessin = sfRectangleShape_create();
                    connexion = inserer_deb_liste(connexion, truc);
                }
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

Un_truc *extraire_deb_liste(Un_elem **liste){
    if(liste == NULL) return NULL;
    Un_elem *t = *liste;
    *liste = (*liste)->suiv;
    t->suiv = NULL;
    Un_truc *truc = t->truc;
    detruire_liste(t);
    return truc;
}

Un_truc *extraire_liste(Un_elem **liste, Un_truc *truc){
    if(liste == NULL) return truc;
    if((*liste)->truc == truc){
        return extraire_deb_liste(liste);
    }

    Un_elem *p = *liste;
    while(p->suiv != NULL && p->suiv->truc != truc){
        p = p->suiv;
    }

    if(p->suiv != NULL){
        Un_elem *s = p->suiv;
        p->suiv = p->suiv->suiv;
        s->suiv = NULL;
    }

    return truc;
}

void dijkstra(Un_elem *liste_sta, Un_truc *sta_dep){
    Un_elem *tete = liste_sta;
    while(tete != NULL){
        tete->truc->user_val = FLT_MAX;
        tete->truc->data.sta.con_pcc = NULL;
        tete = tete->suiv;
    }

    sta_dep->user_val = 0.0;
    sta_dep->data.sta.con_pcc = sta_dep;
    
    Un_elem *Q = NULL;

    tete = liste_sta;

    while(tete != NULL){
        Q = inserer_liste_trie(Q, tete->truc);
        tete = tete->suiv;
    }

    while(Q != NULL){
        Un_truc *current = extraire_deb_liste(&Q);
        for (int i = 0; i < current->data.sta.nb_con; i++){
            Un_truc *connexion = current->data.sta.tab_con[i];
            float duree = current->user_val + connexion->user_val;
            if(duree < connexion->data.con.sta_arr->user_val){
                connexion->data.con.sta_arr->user_val = duree;
                connexion->data.con.sta_arr->data.sta.con_pcc = current;
            }
        }

        tete = Q;
        Q = NULL;

        while(tete != NULL){
            Q = inserer_liste_trie(Q, tete->truc);
            tete = tete->suiv;
        }
    }
}

Un_elem *cherche_chemin(Un_truc *sta_arr){
    if(sta_arr == NULL || sta_arr->data.sta.con_pcc == NULL) return NULL;
    Un_elem *elem = NULL;
    while(sta_arr != NULL && sta_arr->data.sta.con_pcc != sta_arr){
        elem = inserer_deb_liste(elem, sta_arr);
        sta_arr = sta_arr->data.sta.con_pcc;
    }

    if(sta_arr == NULL){
        detruire_liste(elem);
        return NULL;
    }

    return inserer_deb_liste(elem, sta_arr);    
}
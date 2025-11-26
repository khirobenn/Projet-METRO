#include <stdio.h>
#include <stdlib.h>
#include "abr_type.h"
#include "aqrtopo.h"
#include "connexion.h"
#include "coord.h"
#include "ligne.h"
#include "liste.h"
#include "station.h"
#include "truc.h"

#define LARGEUR 1800
#define LONGUEUR 1000

void dessiner_stations(Un_elem *liste, sfRenderWindow *window);
void mettre_a_jour_les_cercles(Un_elem *l);

int main(int argc, char **argv){
    Un_elem *l = lire_stations(argv[1]);
    mettre_a_jour_les_cercles(l);
    ecrire_liste(NULL, l);
    // Un_elem *l = lire_stations("stations.csv");
    Un_noeud *aqr = construire_aqr(l);
    Une_coord cord = {.lon = 2.4840, .lat = 49.2600};
    Un_truc *t = chercher_aqr(aqr, cord);
    if(t != NULL){
        printf("%s\n", t->data.sta.nom);
        sfCircleShape_setFillColor(t->data.sta.point, sfRed);
    }
    else{
        printf("Pas de station\n");
    }

    sfVideoMode mode = {LARGEUR, LONGUEUR, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "MAP", sfResize | sfClose, NULL);

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtClosed) {
            sfRenderWindow_close(window);
        }
        }

        sfRenderWindow_clear(window, sfBlack);
        dessiner_stations(l, window);
        sfRenderWindow_display(window);
    }

    detruire_aqr(aqr);
    detruire_liste_et_truc(l);
    sfRenderWindow_destroy(window);
    return 0;
}

void dessiner_stations(Un_elem *liste, sfRenderWindow *window){
    while(liste != NULL){
        // sfVector2f pos = sfCircleShape_getPosition(liste->truc->data.sta.point);
        // printf("x = %f, y = %f\n", pos.x, pos.y);
        sfRenderWindow_drawCircleShape(window, liste->truc->data.sta.point, NULL);
        liste = liste->suiv;
    }
}

void mettre_a_jour_les_cercles(Un_elem *l){
    Une_coord se, no;
    limites_zone(l, &no, &se);
    float lat_dis = se.lat - no.lat;
    float lon_dis = no.lon - se.lon;
    while(l != NULL){
        sfVector2f pos = {.x = ((l->truc->coord.lat - no.lat) / lat_dis) * LARGEUR,
                          .y = (1 - (l->truc->coord.lon - se.lon) / lon_dis) * LONGUEUR
                        };
        pos.x *= 0.98;
        pos.y *= 0.98;
        sfCircleShape_setPosition(l->truc->data.sta.point, pos);
        l = l->suiv;
    }
}
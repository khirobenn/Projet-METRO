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
#include <string.h>

#define LARGEUR 1800
#define LONGUEUR 900
#define STEP 0.001
#define ZOOM_STEP 0.01
#define SIZE_BETWEEN 0.05

void dessiner_stations(Un_elem *liste, sfRenderWindow *window);
void mettre_a_jour_les_cercles(Un_elem *l, Une_coord se, Une_coord no);

int main(int argc, char **argv){
    Un_elem *l = lire_stations(argv[1]);
    Un_noeud *aqr = construire_aqr(l);
    // print_aqr(aqr);
    Une_coord no = {.lat = 48.8, .lon = 2.2}, se = {.lat = no.lat + SIZE_BETWEEN*2, .lon = no.lon - SIZE_BETWEEN};

    Un_elem *petit_carre = chercher_zone(aqr, NULL, se, no);
    mettre_a_jour_les_cercles(petit_carre, se, no);
    ecrire_liste(NULL, petit_carre);

    
    sfVideoMode mode = {LARGEUR, LONGUEUR, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "MAP", sfResize | sfClose, NULL);
    
    int mousePressed = 0;
    sfVector2i currentMouseCord = sfMouse_getPositionRenderWindow(window);

    sfVector2f tmp = {.x = LARGEUR/2 - 100, .y = LONGUEUR/2 - 100};
    float alpha = 0.2;
    tmp.x = alpha*LARGEUR;
    tmp.y = alpha*LONGUEUR;
    
    tmp.x = 40;
    tmp.y = 40;
    
    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setSize(rect, tmp);
    tmp.x = LARGEUR - tmp.x - 20;
    tmp.y = LONGUEUR - tmp.y - 20;
    sfRectangleShape_setPosition(rect, tmp);
    sfRectangleShape_setFillColor(rect, sfBlack);
    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            else if(event.type == sfEvtMouseWheelScrolled){
                if (event.mouseWheelScroll.delta >= 0){
                    if(no.lon - se.lon <= 1 && se.lat - no.lat <= 0.3){
                        se.lat += ZOOM_STEP;
                        no.lat -= ZOOM_STEP;
                        se.lon -= ZOOM_STEP;
                        no.lon += ZOOM_STEP;
                    }
                }
                else{
                    if(no.lon - se.lon >= SIZE_BETWEEN && se.lat - no.lat >= SIZE_BETWEEN * 2){
                        se.lat -= ZOOM_STEP;
                        no.lat += ZOOM_STEP;
                        se.lon += ZOOM_STEP;
                        no.lon -= ZOOM_STEP;
                    }
                }

            }
            else if(event.type == sfEvtMouseButtonPressed){
                sfFloatRect cord = sfRectangleShape_getLocalBounds(rect);
                sfVector2i m = sfMouse_getPositionRenderWindow(window);
                if(sfFloatRect_contains(&cord, m.x, m.y) == sfTrue){
                    tmp.x = LARGEUR/2 - 100;
                    tmp.y = LONGUEUR/2 - 100;
                    // sfView_setCenter(fenetre, tmp);
                    tmp.x = alpha*LARGEUR;
                    tmp.y = alpha*LONGUEUR;
                    // sfView_setSize(fenetre, tmp);
                    // sfRenderWindow_setView(window, fenetre);
                }
                else{
                    mousePressed = 1;
                }
            }
            else if(event.type == sfEvtMouseButtonReleased){
                mousePressed = 0;
            }
            else if(event.type == sfEvtMouseMoved && mousePressed){
                sfVector2i move = sfMouse_getPositionRenderWindow(window);
                sfVector2f m = {.x = currentMouseCord.x - move.x, .y = currentMouseCord.y - move.y};
                // printf("%lf, %lf\n", m.x, m.y);
                if(m.x > 0){
                    se.lat += STEP;
                    no.lat += STEP;
                }
                else if(m.x < 0){
                    se.lat -= STEP;
                    no.lat -= STEP;
                }

                if(m.y < 0){
                    se.lon += STEP;
                    no.lon += STEP;
                }
                else if(m.y > 0){
                    se.lon -= STEP;
                    no.lon -= STEP;
                }
                // sfView_move(fenetre, m);
                // sfRenderWindow_setView(window, fenetre);  
            }

            detruire_liste(petit_carre);
            petit_carre = chercher_zone(aqr, NULL, se, no);
            mettre_a_jour_les_cercles(petit_carre, se, no);
            printf("\n");
            ecrire_liste(NULL, petit_carre);
            
        }
        currentMouseCord = sfMouse_getPositionRenderWindow(window);

        // sfRenderWindow_setView(window, fenetre);
        sfRenderWindow_clear(window, sfWhite);
        dessiner_stations(petit_carre, window);
        sfRenderWindow_setView(window, sfRenderWindow_getDefaultView(window));
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
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
        sfRenderWindow_drawText(window, liste->truc->data.sta.nom_shape, NULL);
        liste = liste->suiv;
    }
}

void mettre_a_jour_les_cercles(Un_elem *l, Une_coord se, Une_coord no){
    if(l == NULL) return;
    sfFont *font = sfFont_createFromFile("Poppins-Regular.ttf");
    float lat_dis = se.lat - no.lat;
    float lon_dis = no.lon - se.lon;
    while(l != NULL){
        sfVector2f pos = {.x = ((l->truc->coord.lat - no.lat) / lat_dis) * LARGEUR,
                          .y = (1 - (l->truc->coord.lon - se.lon) / lon_dis) * LONGUEUR
                        };
        printf("%lf, %lf\n", pos.x, pos.y);
        pos.x *= 0.98;
        pos.y *= 0.98;
        sfCircleShape_setPosition(l->truc->data.sta.point, pos);
        pos.x -= strlen(l->truc->data.sta.nom);
        pos.y -= 8;
        sfText_setPosition(l->truc->data.sta.nom_shape, pos);
        sfText_setFont(l->truc->data.sta.nom_shape, font);
        sfText_setCharacterSize(l->truc->data.sta.nom_shape, 8);
        l = l->suiv;
    }
}
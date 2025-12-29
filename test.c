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
#include <math.h>
#include "menu.h"

#define LARGEUR 1800
#define LONGUEUR 900
#define STEP 0.001
#define ZOOM_STEP 0.01
#define SIZE_BETWEEN 0.05

#define PI 3.14159265358979323846

void dessiner_stations(Un_elem *liste, sfRenderWindow *window);
void mettre_a_jour_les_cercles(Un_elem *l, Une_coord se, Une_coord no);
void dessiner_lignes(Un_elem *l, sfRenderWindow *window, Une_coord se, Une_coord no);
int conversion_hex_vers_decimal(char *depart);

int main(int argc, char **argv){
    Un_elem *l = lire_stations(argv[1]);
    Un_noeud *aqr = construire_aqr(l);
    Un_nabr *abr = construire_abr(l);

    Une_ligne *lignes = lire_lignes(argv[2]);


    Un_elem *test = lire_connexions(argv[3], lignes, abr);

    if(test != NULL){
        printf("OKKK!!\n");
    }

    // print_aqr(aqr);
    Une_coord no = {.lat = 48.8, .lon = 2.3}, se = {.lat = no.lat + SIZE_BETWEEN*2, .lon = no.lon - SIZE_BETWEEN};

    Un_elem *petit_carre = chercher_zone(aqr, NULL, se, no);
    mettre_a_jour_les_cercles(petit_carre, se, no);
    
    sfVideoMode mode = {LARGEUR, LONGUEUR, 32};

    int choix = -1;

    do{
        choix = menu();
        if(choix == 2){
            printf("\33[2J");
            printf("\33[H");
            printf("Entrez le nom de la station que vous cherchez : ");
            // curs_set(1);
            printf("\33[?25h");
            char nom[200];
            scanf(" %[^\n]", nom);
            refresh();
            fflush(stdout);

            Un_truc *station_recherche = chercher_station(abr, nom);
            if(station_recherche != NULL){
                printf("Station trouvée !!\33[1E");
                no.lat = station_recherche->coord.lat - SIZE_BETWEEN;
                se.lat = station_recherche->coord.lat + SIZE_BETWEEN;

                no.lon = station_recherche->coord.lon + SIZE_BETWEEN/2;
                se.lon = station_recherche->coord.lon - SIZE_BETWEEN/2;
                choix = 1;
            }
            station_recherche = NULL;
        }

        if(choix == 1){
            sfRenderWindow *window = sfRenderWindow_create(mode, "MAP", sfResize | sfClose, NULL);
            int mousePressed = 0;
            sfVector2i currentMouseCord = sfMouse_getPositionRenderWindow(window);
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
                        mousePressed = 1;
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
                }
                currentMouseCord = sfMouse_getPositionRenderWindow(window);
        
                // sfRenderWindow_setView(window, fenetre);
                sfRenderWindow_clear(window, sfWhite);
                dessiner_lignes(test, window, se, no);
                dessiner_stations(petit_carre, window);
                sfRenderWindow_setView(window, sfRenderWindow_getDefaultView(window));
                sfRenderWindow_display(window);
            }
            sfRenderWindow_destroy(window);
        }
    }while(choix != NB);


    detruire_liste(petit_carre);
    detruire_abr(abr);
    detruire_lignes(lignes);
    detruire_aqr(aqr);
    detruire_liste(test);
    detruire_liste_et_truc(l);

    printf("\33[2J");
    printf("\33[H");
    printf("\33[?25h");

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

void dessiner_lignes(Un_elem *l, sfRenderWindow *window, Une_coord se, Une_coord no){
    float lat_dis = se.lat - no.lat;
    float lon_dis = no.lon - se.lon;

    while(l != NULL){
        Une_coord station1 = l->truc->data.con.sta_dep->coord;
        Une_coord station2 = l->truc->data.con.sta_arr->coord;


        sfVector2f pos1 = {.x = ((station1.lat - no.lat) / lat_dis) * LARGEUR,
                          .y = (1 - (station1.lon - se.lon) / lon_dis) * LONGUEUR
                        };

        sfVector2f pos2 = {.x = ((station2.lat - no.lat) / lat_dis) * LARGEUR,
                          .y = (1 - (station2.lon - se.lon) / lon_dis) * LONGUEUR
                        };

        double x = fabs(pos1.x - pos2.x);
        double y = fabs(pos1.y - pos2.y);

        double dis = sqrt(x*x + y*y);
 
        sfVector2f size = {.x = dis, .y = 2};
        sfRectangleShape_setSize(l->truc->data.con.ligne_dessin, size);
        char *color = l->truc->data.con.ligne->color;

        sfColor colorFill = {.r = conversion_hex_vers_decimal(color+1), .g = conversion_hex_vers_decimal(color+3), .b = conversion_hex_vers_decimal(color+5), .a = conversion_hex_vers_decimal(color+7)};
        sfRectangleShape_setFillColor(l->truc->data.con.ligne_dessin, colorFill);
        sfRectangleShape_setPosition(l->truc->data.con.ligne_dessin, pos1);

        double teta = atan(y/x) * 180.0 / PI;

        if(station2.lat > station1.lat && station2.lon > station1.lon){
            teta = -teta;
        }
        else if(station2.lat < station1.lat && station2.lon < station1.lon){
            teta = 180 - teta;
        }
        else if(station2.lat < station1.lat && station2.lon > station1.lon){
            teta -= 180;
        }
        sfRectangleShape_setRotation(l->truc->data.con.ligne_dessin, teta);
        sfRenderWindow_drawRectangleShape(window, l->truc->data.con.ligne_dessin, NULL);
        l = l->suiv;
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
        sfCircleShape_setPosition(l->truc->data.sta.point, pos);
        pos.x -= strlen(l->truc->data.sta.nom);
        pos.y -= 8;
        sfText_setPosition(l->truc->data.sta.nom_shape, pos);
        sfText_setFont(l->truc->data.sta.nom_shape, font);
        sfText_setCharacterSize(l->truc->data.sta.nom_shape, 8);
        l = l->suiv;
    }
}

int conversion_hex_vers_decimal(char *depart){
    int dec = 0;
    for (int i = 0; i<2; i++){
        switch(depart[0]){
            case 'A':
            case 'a':
                dec = dec*16 + 10;
                break;
            case 'B':
            case 'b':
                dec = dec*16 + 11;
                break;
            case 'C':
            case 'c':
                dec = dec*16 + 12;
                break;
            case 'D':
            case 'd':
                dec = dec*16 + 13;
                break;
            case 'E':
            case 'e':
                dec = dec*16 + 14;
                break;
            case 'F':
            case 'f':
                dec = dec*16 + 15;
                break;
            default:
                dec = dec*16 + (depart[0] - '0');
        }

        depart++;
    }

    return dec;
}
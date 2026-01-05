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
#define STEP 0.002
#define ZOOM_STEP 0.01
#define SIZE_BETWEEN 0.05

#define PI 3.14159265358979323846

void dessiner_tout(Une_coord no, Une_coord se, Un_elem **petit_carre, Un_noeud *aqr, Un_elem *liste_des_connexions);
void dessiner_stations(Un_elem *liste, sfRenderWindow *window, Une_coord se, Une_coord no);
void mettre_a_jour_les_cercles(Un_elem *l, Une_coord se, Une_coord no);
void dessiner_lignes(Un_elem *l, sfRenderWindow *window, Une_coord se, Une_coord no);
int conversion_hex_vers_decimal(char *depart);
void affichage_iteniraire(Un_elem *l, int direction);

int main(int argc, char **argv){
    Un_elem *l = lire_stations(argv[1]);
    Un_noeud *aqr = construire_aqr(l);
    Un_nabr *abr = construire_abr(l);

    Une_ligne *lignes = lire_lignes(argv[2]);

    Un_elem *liste_des_connexions = lire_connexions(argv[3], lignes, abr);

    // print_aqr(aqr);
    Une_coord no = {.lat = 48.75, .lon = 2.4}, se = {.lat = no.lat + SIZE_BETWEEN*4, .lon = no.lon - 2*SIZE_BETWEEN};

    Un_elem *petit_carre = chercher_zone(aqr, NULL, se, no);
    mettre_a_jour_les_cercles(petit_carre, se, no);
    
    int choix = -1;

    do{
        choix = menu();
        if(choix == 1){
            no.lat = 48.75;
            no.lon = 2.4;
            se.lat = no.lat + SIZE_BETWEEN*4;
            se.lon = no.lon - SIZE_BETWEEN*2;
            dessiner_tout(no, se, &petit_carre, aqr, liste_des_connexions);
        }
        else if(choix == 2){
            printf("\33[2J");
            printf("\33[H");
            printf("Entrez le nom de la station que vous cherchez : ");
            // Pour remettre le cursor
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
                dessiner_tout(no, se, &petit_carre, aqr, liste_des_connexions);  
            }
            else{
                printf("Veuillez saisir un nom de station valide !");
                printf("\33[1E");
                printf("Appuyez sur une touche pour retourner au menu :)");
                refresh();
                fflush(stdout);
                getch();
            }
        }
        else if(choix == 3){
            printf("Entrez le nom de la station de départ : ");
            // Pour remettre le cursor
            printf("\33[?25h");
            char nom[200];
            scanf(" %[^\n]", nom);
            Un_truc *depart = chercher_station(abr, nom);

            printf("Entrez le nom de la station d'arrivé : ");
            scanf(" %[^\n]", nom);
            Un_truc *arrive = chercher_station(abr, nom);


            printf("\33[2J");
            printf("\33[H");
            refresh();
            fflush(stdout);

            int changement = 0;
            if(depart == NULL || arrive == NULL){
                printf("Veuillez saisir des noms de stations valides !");
                printf("\33[1E");
            }
            else{
                dijkstra(l, depart);
                Un_elem *chemin = cherche_chemin(arrive);
                if(chemin == NULL){
                    dijkstra(l, arrive);
                    chemin = cherche_chemin(depart);
                    changement = 1;
                }

                if(chemin != NULL){
                    float temps = 0.0;
                    if(changement) temps = depart->user_val;
                    else temps = arrive->user_val;

                    affichage_iteniraire(chemin, changement);
                    printf("\33[1E");
                    printf("Temps total : %.2lf minutes !", temps);
                    printf("\33[1E");

                }


                detruire_liste(chemin);
            }
            
            printf("\33[1E");
            printf("Appuyez sur une touche pour retourner au menu :)");
            refresh();
            fflush(stdout);
            getch();
        }

    }while(choix != NB);


    detruire_liste(petit_carre);
    detruire_abr(abr);
    detruire_lignes(lignes);
    detruire_aqr(aqr);
    detruire_liste(liste_des_connexions);
    detruire_liste_et_truc(l);

    printf("\33[2J");
    printf("\33[H");
    printf("\33[?25h");

    return 0;
}


void dessiner_tout(Une_coord no, Une_coord se, Un_elem **petit_carre, Un_noeud *aqr, Un_elem *liste_des_connexions){
    sfVideoMode mode = {LARGEUR, LONGUEUR, 32};

    sfRenderWindow *window = sfRenderWindow_create(mode, "MAP", sfResize | sfClose, NULL);
    int mousePressed = 0;
    sfVector2i currentMouseCord = sfMouse_getPositionRenderWindow(window);
    while (sfRenderWindow_isOpen(window)) {
        printf("NO : lat = %lf ; lon = %lf\n", no.lat, no.lon);
        printf("SE : lat = %lf ; lon = %lf\n", se.lat, se.lon);
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
                        detruire_liste(*petit_carre);
                        *petit_carre = chercher_zone(aqr, NULL, se, no);
                    }
                }
                else{
                    if(no.lon - se.lon >= 0.065 && se.lat - no.lat >= 0.165){
                        se.lat -= ZOOM_STEP;
                        no.lat += ZOOM_STEP;
                        se.lon += ZOOM_STEP;
                        no.lon -= ZOOM_STEP;
                        detruire_liste(*petit_carre);
                        *petit_carre = chercher_zone(aqr, NULL, se, no);
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
                detruire_liste(*petit_carre);
                *petit_carre = chercher_zone(aqr, NULL, se, no);
            }
            mettre_a_jour_les_cercles(*petit_carre, se, no);
        }
        currentMouseCord = sfMouse_getPositionRenderWindow(window);

        sfRenderWindow_clear(window, sfWhite);
        dessiner_lignes(liste_des_connexions, window, se, no);
        dessiner_stations(*petit_carre, window, se, no);
        sfRenderWindow_setView(window, sfRenderWindow_getDefaultView(window));
        sfRenderWindow_display(window);
    }
    sfRenderWindow_destroy(window);
}

void dessiner_stations(Un_elem *liste, sfRenderWindow *window, Une_coord se, Une_coord no){
    float lat_dis = se.lat - no.lat;
    float lon_dis = no.lon - se.lon;

    if(lat_dis > 0.17 || lon_dis > 0.07 ) return;
    while(liste != NULL){
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
 
        sfVector2f size = {.x = dis, .y = 3};
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
        pos.y -= 12;
        sfText_setPosition(l->truc->data.sta.nom_shape, pos);
        sfText_setFont(l->truc->data.sta.nom_shape, font);
        sfText_setCharacterSize(l->truc->data.sta.nom_shape, 10);
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

void affichage_iteniraire(Un_elem *l, int direction){
    if(l == NULL) return;
    if (direction == 0){
        printf("%s", l->truc->data.sta.nom);
        if(l->suiv != NULL){
            printf(" --> ");
        }
        affichage_iteniraire(l->suiv, direction);
    }
}
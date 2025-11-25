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
#include <SFML/Graphics.h>

int main(int argc, char **argv){
    Un_elem *l = lire_stations(argv[1]);
    // Un_elem *l = lire_stations("stations.csv");
    Un_noeud *aqr = construire_aqr(l);

    sfVideoMode mode = {800, 450, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "CSFML Example", sfResize | sfClose, NULL);

    sfRectangleShape *shape = sfRectangleShape_create();
    sfVector2f vf = {200, 200}, pos = {30, 30};
    sfRectangleShape_setSize(shape, vf);
    sfRectangleShape_setFillColor(shape, sfGreen);
    sfRectangleShape_setPosition(shape, pos);

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtClosed) {
            sfRenderWindow_close(window);
        }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawRectangleShape(window, shape, NULL);
        sfRenderWindow_display(window);
    }

    sfRectangleShape_destroy(shape);
    sfRenderWindow_destroy(window);


    // if(argc <= 3){
    //     printf("Erreur : ./test : stations.csv lignes.csv connexions.csv\n");
    //     return 1;
    // }
    // printf("------------------------------\n");
    // Une_ligne *ligne = lire_lignes(argv[2]);
    // afficher_lignes(ligne);

    // printf("------------------------------\n");

    // Un_nabr *stations = construire_abr(l);
    // print_arb(stations);
    // printf("------------------------------\n");

    // char code[] = "La Defense";
    // Un_truc *t = chercher_station(stations, code);
    // if(t == NULL){
    //     printf("Pas de station avec le nom : %s\n", code);
    // }
    // else{
    //     printf("%s est la !!\n", t->data.sta.nom);
    //     printf("%f, %f\n", t->coord.lat, t->coord.lon);
    // }
    // printf("------------------------------\n");

    
    // Un_elem *connexion = lire_connexions(argv[3], ligne, stations);
    // ecrire_liste(NULL, connexion);
    // printf("------------------------------\n");

    // if(aqr == NULL){
    //     printf("aqr vide\n");
    // }
    // printf("\n");
    // printf("\n");

    // print_aqr(aqr);
    // printf("\n");

    // // Coordonnées de Robinson
    // Une_coord coord = {.lat = 72.0, .lon = 22.0};
    // Un_truc * truc = chercher_aqr(aqr, coord);
    // if(truc != NULL){
    //     printf("%s\n", truc->data.sta.nom);
    // }
    // else{
    //     printf("Pas de station à ces coord!\n");
    // }
    // printf("------------------------------\n");

    // Une_coord sud_est, nord_ouest;
    // limites_zone(l, &nord_ouest, &sud_est);
    // printf("NO : lat : %lf, lon : %lf\n", nord_ouest.lat, nord_ouest.lon);
    // printf("SE : lat : %lf, lon : %lf\n", sud_est.lat, sud_est.lon);
    // Une_coord se = {.lat = 75, .lon = 22};
    // Une_coord no = {.lat = 72, .lon = 25};
    // Un_elem *zone = chercher_zone(aqr, NULL, se, no);
    // printf("-------------------------------\n");
    // ecrire_liste(NULL, zone);

    // detruire_liste(connexion);
    // detruire_abr(stations);
    // detruire_lignes(ligne);
    // detruire_liste(zone);
    detruire_aqr(aqr);
    detruire_liste_et_truc(l);
    return 0;
}
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

int main(int argc, char **argv){
    if(argc <= 1){
        printf("Erreur : ./test : arg1 arg2 ..\n");
        return 1;
    }
    Un_elem *l = lire_stations(argv[1]);
    ecrire_liste(NULL, l);
    printf("------------------------------\n");
    Une_ligne *ligne = lire_lignes(argv[2]);
    afficher_lignes(ligne);

    printf("------------------------------\n");

    Un_nabr *stations = construire_abr(l);
    print_arb(stations);
    printf("------------------------------\n");

    char code[] = "Denfert khiro";
    Un_truc *t = chercher_station(stations, code);
    if(t == NULL){
        printf("Pas de station avec le nom : %s\n", code);
    }
    else{
        printf("%s est la !!\n", t->data.sta.nom);
    }
    printf("------------------------------\n");

    Un_elem *connexion = lire_connexions(argv[3], ligne, stations);
    ecrire_liste(NULL, connexion);
    detruire_liste(connexion);
    detruire_abr(stations);
    detruire_lignes(ligne);
    detruire_liste_et_truc(l);
    return 0;
}
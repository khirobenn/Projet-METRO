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

int main(){
    Un_elem *l = lire_stations("test.csv");
    ecrire_liste(NULL, l);
    detruire_liste_et_truc(l);
    return 0;
}
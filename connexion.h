#ifndef CONNEXION_H
#define CONNEXION_H

#include <SFML/Graphics.h>

typedef struct _une_connexion {
    struct _un_truc *sta_dep; //Station de depart
    struct _un_truc *sta_arr; //Station d arrivee
    struct _une_ligne *ligne; //Ligne
    sfRectangleShape *ligne_dessin;
} Une_connexion;

#endif
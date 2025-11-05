#include <stdio.h>
#include "ligne.h"
#include <string.h>
#include <stdlib.h>

#define CODE_TAILLE 8
#define COULEUR_TAILLE 8

Une_ligne *lire_lignes(char *nom_fichier){
    FILE *f = fopen(nom_fichier, "r");
    char code[CODE_TAILLE];
    char couleur[COULEUR_TAILLE];
    float vitesse;
    float intervalle;

    Une_ligne *ligne = NULL;
    Une_ligne *ligne_ptr;
    
    while(fscanf(f, "%s ; %f ; %f ; %s", code, &vitesse, &intervalle, couleur) != EOF){
        Une_ligne *nouvelle_ligne = malloc(sizeof(Une_ligne));
        
        char *ligne_code = malloc(sizeof(char)*CODE_TAILLE);
        strcpy(ligne_code, code);
        
        char *ligne_couleur = malloc(sizeof(char)*COULEUR_TAILLE);
        strcpy(ligne_couleur, couleur);
        
        nouvelle_ligne->code = ligne_code;
        nouvelle_ligne->color = ligne_couleur;
        nouvelle_ligne->vitesse = vitesse;
        nouvelle_ligne->intervalle = intervalle;
        nouvelle_ligne->suiv = NULL;

        if(ligne == NULL){
            ligne = nouvelle_ligne;
            ligne_ptr = ligne;
        }
        else{
            ligne_ptr->suiv = nouvelle_ligne;
            ligne_ptr = ligne_ptr->suiv;
        }
    }

    fclose(f);
    return ligne;
}

void afficher_lignes(Une_ligne *lligne){
    while(lligne != NULL){
        printf("%s ; %f ; %f ; %s", lligne->code, lligne->vitesse, lligne->intervalle, lligne->color);
        lligne = lligne->suiv;
    }
}

void detruire_lignes(Une_ligne *lligne){
    if(lligne == NULL) return;
    Une_ligne *reste = lligne->suiv;
    lligne->suiv = NULL;
    free(lligne->code);
    free(lligne->color);
    free(lligne);
    detruire_lignes(reste);
}

Une_ligne *chercher_ligne(Une_ligne *lligne, char *code){
    if(lligne == NULL) return NULL;
    if(strcmp(lligne->code, code) == 0) return lligne;
    return chercher_ligne(lligne->suiv, code);
}
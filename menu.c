#include "menu.h"

int menu(){
    int key_menu;
    int choix = 1;
    char *table_de_choix[4] = {
        "               Afficher la carte               ",
        "              Chercher une station             ",
        "             Chercher un itinéraire            ",
        "                    Quitter                    ",};
    
    char *table_de_mode[4] = {
    " Afficher la carte",
    " Chercher une station",
    " Chercher un itinéraire",
    " Quitter",};
    initscr();
    refresh();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    do{
        printf("\33[2J");
        printf("\33[H");
        printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s MENU %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s","\u2554" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550"  ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2557");
        printf("\33[1E");
        for(int i=0; i<NB; i++){
            printf("%s", "\u2551");
            if(choix == i+1) printf("\33[5m\33[45m%s\33[00m", table_de_choix[i]);
            else printf("%s", table_de_choix[i]);
            printf("%s\33[1E", "\u2551");
        }
        printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s","\u255a" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" , "\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550"  ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u2550" ,"\u255d");
        printf("\33[1E\33[1E");
        printf("Vous êtes sur le point de choisir %s \33[1;35m%s\33[00m\33[1E", "\u25B6", table_de_mode[choix-1]);
        printf("%s Cliquez sur 'entrer' pour confirmer votre choix.\33[1E\33[1E", "\u25B6");
        
        printf("\33[?25l");

        fflush(stdout);
        key_menu = getch();
        if(key_menu == KEY_UP){choix--; if(choix == 0) choix = NB;}
        else if(key_menu == KEY_DOWN){choix++; if(choix == NB+1) choix = 1; }

    }while(key_menu != 10);

    endwin();
    return choix;
}
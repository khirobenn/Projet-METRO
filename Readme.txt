# Bibliothéques à installer avant la compilation :
## 1. CSFML :
[CSFML](https://www.sfml-dev.org/download/csfml/index-fr.php) est le binding officiel de SFML pour le langage C. Son API est aussi proche que possible que l'API C++ (mais dans le style C, bien entendu), ce qui en fait un candidat parfait pour construire des bindings SFML pour d'autres langages qui ne supportent pas directement les bibliothèques C++.

Pour l'installer, il suffit de lancer cette commande sur le terminal `sudo apt install libcsfml-*`

## 2. NCurses :
Ncurses est une bibliothèque qui fournit des fonctions de définition de touches du clavier, de couleurs d'écran et permet l'affichage de plusieurs fenêtres sans recouvrement sur un terminal texte.

Pour l'installer, il faut lancer la commande suivante sur le terminal `sudo apt-get install libncurses5-dev libncursesw5-dev`

# Compilation et exécution :
## 1. Compilation :
On exécute la commande `make` et c'est bon tout est compilé.

## 2. Exécution :
Pour l'exécution il faut lancer le programme `test` avec les arguments suivants avec cet ordre `stations.csv lignes.csv connexions.csv`. 

C'est à dire faut l'exécuter de cette façon : `./test stations.csv lignes.csv connexions.csv`.

# Utilisation du programme :
Lors de l'exécution du programme, on aura une interface graphique sur le terminal où on pourra se déplacer dans le menu avec les touches `haut` et `bas`, et on utilise la touche `entrer` pour séléctionner notre choix.

Tout les choix, ouvrent une fenêtre où on affichera toutes les villes qui se trouvent dans le fichier `stations.csv`.

Pour retourner vers le menu sur le terminal, c'est à dire le menu des choix, il suffit de fermer la fenêtre ouverte dans le précedent choix avec la croix rouge en haut à droite.

Enfin, pour arrêter le programme, il faut choisir le choix 'Quitter' sur le menu du terminal.

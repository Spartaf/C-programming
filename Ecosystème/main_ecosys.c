#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"


#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 30000
#define NB_ITERATIONS 500
#define ENERGIE_PREDATEUR 20
#define ENERGIE_PROIE 20

/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.01; // Proba de changer de direction
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;
/* Ce paramètre rajoute une probabilité pour un prédateur de manger ou non la proie sur sa case */
float p_manger = 1; 
/* Ce paramètre rajoute la possibilité que le prédateur ne mange sa proie que si il possède plus d'énergie quelle */
int manger_ssi_engergie = 0; // 0 si le paremètre est désactivé 1 sinon

// Pour afficher les graphiques
  // A taper dans cet ordre :
  /*
  gnuplot
  set xlabel 'generations';set ylabel 'Population'
  plot "Evol_pop.txt" using 1:2 with lines title "proies"
  replot "Evol_pop.txt" using 1:3 with lines title "predateurs"
  */

// Utilisation de Make
// "make clean" pour effacer les .o
// "make" pour tout compiler d'un coup 


int main(void) {
  srand(50);

  int monde[SIZE_X][SIZE_Y];
  int i, j;

  // Initialisation du monde 

  for(i = 0; i < SIZE_X; i++){
    for(j = 0; j < SIZE_Y; j++){
      monde[i][j] = 0;
    }
  }

  // Initialisation des animaux

  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  for(int i = 0; i < NB_PROIES; i++){
    ajouter_animal(rand()%SIZE_X,rand()%SIZE_Y,ENERGIE_PROIE,&liste_proie);
    ajouter_animal(rand()%SIZE_X,rand()%SIZE_Y,ENERGIE_PREDATEUR,&liste_predateur);
  }

  /*Possibilité de lire un fichier text pour la création des animaux*/
  ecrire_ecosys("ecrire_ecosys.txt" , liste_predateur , liste_proie);

  // Creation du monde depuis un fichier text
  //lire_ecosys("lire_ecosys.txt" , &liste_proie , &liste_predateur);

  // Supression du fichier de graphique deja existant
  FILE* fichier = fopen("Evol_pop.txt" ,"r");
  if (fichier != NULL){
    remove("Evol_pop.txt");
  }

  // Evolution de l'écosystème 
  clear_screen();
  afficher_ecosys(liste_proie , liste_predateur);

  for(int i = 0; i <= NB_ITERATIONS; i++){

    if((compte_animal_rec(liste_proie) <= 0) || (compte_animal_rec(liste_predateur) <= 0)){
      printf("Il n'y a plus de prédateurs ou de proies\n");
      break;
    }

    // Rafraichissement du monde 
    rafraichir_monde( monde );

    // Rafraichissement des proies
    rafraichir_proies( &liste_proie , monde );

    // Rafraichissement des prédateurs
    rafraichir_predateurs( &liste_predateur , &liste_proie );

    // Rafraichissement de l'affichage
    usleep(T_WAIT);

    afficher_ecosys( liste_proie , liste_predateur );
    printf("Itération : %d\n" , i);

    // Ecriture du graphique
    ecrire_graphique("Evol_pop.txt" , i , liste_predateur , liste_proie);
  }

  // Pour afficher les graphiques
  // A taper dans cet ordre :
  /*
  gnuplot
  set xlabel 'generations';set ylabel 'Population'
  plot "Evol_pop.txt" using 1:2 with lines title "proies"
  replot "Evol_pop.txt" using 1:3 with lines title "predateurs"
  */
  
 

  // Libération de la mémoire 
  liberer_liste_animaux( liste_proie );
  liberer_liste_animaux( liste_predateur );

  return 0;
}

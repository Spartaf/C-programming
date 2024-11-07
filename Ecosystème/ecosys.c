#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecosys.h"


// Creation d'un nouvel animal
Animal *creer_animal(int x, int y, float energie) {
  Animal *new = (Animal *)malloc(sizeof(Animal));
  assert(new);
  new->x = x;
  new->y = y;
  new->energie = energie;
  new->dir[0] = rand() % 3 - 1;
  new->dir[1] = rand() % 3 - 1;
  new->suivant = NULL;
  return new;
}

// Ajoute en tete dans liste des animaux
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

// Creer et ajoute l'animal à la liste des animaux
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  Animal* new = NULL;
  new = creer_animal(x,y,energie);
  *liste_animal = ajouter_en_tete_animal( *liste_animal, new);
}

// Supprime l'animal de la liste des animaux
void enlever_animal(Animal **liste, Animal *animal) {
  Animal* copie1 = *liste;
  if (copie1 == animal){
    *liste = copie1->suivant;
    free(animal);
  }
  else{
    Animal*copie = *liste;
    while(copie){
      if(copie->suivant == animal){
        Animal* temp2 = animal->suivant;
        free(animal);
        copie->suivant = temp2;
      }
      copie = copie -> suivant;
    }
  }
  return ;
}
// Libere la liste des animaux
Animal* liberer_liste_animaux(Animal *liste) {
  Animal* temp = NULL;
  while(liste){
    temp = liste -> suivant;
    free(liste);
    liste = temp;
  }
  return NULL;
}
// Fonction qui compte le nombre d'animaux dans la liste d'animaux de maniere itérative
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}
// Fonction qui compte le nombre d'animaux dans la liste d'animaux de maniere recursive
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}





// -------------------------- FONCTION D'AFFICHAGE -------------------------------------






// Cette fonction est une fonction d'affichage de l'environnement
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}
// cette fonction nettoie tout le teminal cest juste un clear()
void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}


//Cette fonction ecris dans un fichier txt l ensemble des animaux avec leur position, leur direction, leur energie en temps reels
void ecrire_ecosys(char *mon_fichier, Animal*liste_predateur, Animal*liste_proie){
    FILE *f = fopen(mon_fichier, "w");
    if (f == NULL){
        fprintf(stderr , "Erreur lors de la lecture \n");
        return;
    }

    // ecriture des proies

    fprintf(f , "<proies>\n");
    while(liste_proie){
        fprintf(f , "x = %d y = %d dir = [%d %d ] e = %f\n" , liste_proie->x , liste_proie -> y , liste_proie -> dir[0] , liste_proie -> dir[1] , liste_proie ->energie);
        liste_proie = liste_proie -> suivant;
    }
    fprintf(f , "</proies>\n");

    // ecriture des prédateurs
    
    fprintf(f , "<predateurs>\n");
    while(liste_predateur){
        fprintf(f , "x = %d y = %d dir = [%d %d ] e = %f\n" , liste_predateur->x , liste_predateur -> y , liste_predateur -> dir[0] , liste_predateur -> dir[1] , liste_predateur ->energie);
        liste_predateur = liste_predateur -> suivant;
    }
    fprintf(f , "</predateurs>\n");


    fclose(f);
}

// Cette fonction permet de creer un environement à partir d'une liste deprédateur et de proies voir le fichier lire ecosys
void lire_ecosys(char *mon_fichier , Animal **liste_proie , Animal **liste_predateur){
  char buffer[64];
  FILE*f = fopen(mon_fichier , "r");
  if (f == NULL){
      fprintf(stderr , "Erreur lors de la lecture \n");
      return;
  }
  if(fgets(buffer,64,f)==NULL){
    fprintf(stderr,"Fichier vide /n");
    fclose(f);
    return;
  }
  if (strcmp("<proies>\n" , buffer) && strcmp("<predateurs>\n" , buffer)){
      fprintf(stderr , "Erreur ce fichier ne contient la balise proies ou de balise prédateurs \n");
      fclose(f);
      return;
  }

  int state ;
  if (strcmp("<proies>\n" , buffer) == 0){
    state = 0;
  }else{
    state = 1;
  }

  int meet = 1;

  while(1){
    int x, y, dir0, dir1;
    float e; 
    Animal * pA;
    Animal * preA;

    if(fgets(buffer, 64, f)== NULL){
      fprintf(stderr , "Erreur fichier");
      fclose(f);
      return;
    }

    if((strcmp(buffer, "<predateurs>\n") == 0)){
      state = 1;
      meet++;
    }
    if((strcmp(buffer, "<proies>\n") == 0)){
      state = 0;
      meet++;
    }
    if( ((strcmp(buffer, "</predateurs>\n") == 0) && (meet==2)) || ((strcmp(buffer, "</proies>\n") == 0) && (meet == 2)) ) {
      break;
    }
    
    if(!(strcmp(buffer, "</proies>\n")==0) && !(strcmp(buffer, "<proies>\n") == 0) && !(strcmp(buffer, "<predateurs>\n") == 0) && !(strcmp(buffer, "</predateurs>\n") == 0)){
      if(sscanf(buffer, "x = %d y = %d dir = [%d %d ] e = %f\n", &x , &y , &dir0 , &dir1 , &e) != 5){
        fprintf(stderr , "Erreur de lecture \n");
        fclose(f);
        return;
      }
      if (state == 0){
        pA = creer_animal(x,y,e);
        pA -> dir[0] = dir0;
        pA -> dir[1] = dir1;
        *liste_proie = ajouter_en_tete_animal(*liste_proie , pA);
      }
      if (state == 1){
        preA = creer_animal(x,y,e);
        preA -> dir[0] = dir0;
        preA -> dir[1] = dir1;
        *liste_predateur = ajouter_en_tete_animal(*liste_predateur , preA);

      } 
    }
  }
  fclose(f);
}

// cette fonction permet d'ecrire dans un fichier la liste de l'évolution du nombre danimaux au cours du temps dans un fichier txt
// cela permettra par la suite d'afficher le graphique avec plot de C
void ecrire_graphique(char* mon_graphique , int iteration , Animal*liste_predateur, Animal*liste_proie){
  FILE *f = fopen(mon_graphique, "a");
  if (f == NULL){
      fprintf(stderr , "Erreur lors de la lecture du fichier de graphique\n");
      return;
  }
  fprintf(f, "%d %d %d\n" , iteration , compte_animal_it(liste_proie) , compte_animal_it(liste_predateur));
  fclose(f);
}




// ----------------------------------- GESTION DES ANIMAUX --------------------------------------

// Cette fonction bouge les animaux en fonction de leur position et direction ainsi que selon la proba de bouger
void bouger_animaux(Animal *la) {
    while(la){
      if ((rand()/(RAND_MAX + 1.0) < p_ch_dir)){
        la -> dir[0] = rand()% 3 - 1;
        la -> dir[0] = rand()% 3 - 1;
      }
      if (la -> dir[0] == 1){
        la -> x = (la ->x + 1)% SIZE_X;
      }
      if (la -> dir[1] == 1){
        la -> y = (la -> y + 1)% SIZE_Y;
      }
      if (la -> dir[0] == -1){
        la -> x = abs((la -> y -1)%SIZE_X);
      }
      if (la -> dir[1] == -1){
        la -> y = abs((la -> y - 1)% SIZE_Y);
      }
      la = la->suivant;
    }
}


// Cette fonction permet de faire reproduire les animaux selon une proba de reprodution fixée par : p_reproduce
// L'énergie de l'enfant est celle du parent divisée par 2
// On divise l'energie du parent par 2 
void reproduce(Animal **liste_animal, float p_reproduce) {
    Animal*copie = *liste_animal;
    if(liste_animal == NULL){return;}
    while(copie){
        if (((float)rand() / (float)RAND_MAX )< p_reproduce){
        ajouter_animal(copie->x,copie->y,(copie->energie)/2,liste_animal);
        copie->energie = (copie->energie)/2;
        }
    copie=copie->suivant;
    }
}

// On rafraichi les proies
// On déplace les proies et on leur fait perdre de l'énergie
// On supprime celle qui n'ont plu d'energie
// On verifie l'emplacement de l'animal : si il y a de l'herbe il la mange et gagne de l'énergie
// L'herbe étant coupée on lui met un temps de repousse négatif qui sera incrémenté à chaque itération de la simulation 
// On fait reproduire la proie toujours selon la proba p_reproduce_proies
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
  Animal*copie1 = *liste_proie;
  Animal*copie2 = *liste_proie;
  bouger_animaux(copie1);
  while(copie2){
    copie2 -> energie = (copie2 -> energie) - 1;

    if(copie2 -> energie < 0){
      Animal *tmp = copie2;
      copie2 = copie2->suivant;
      enlever_animal(liste_proie , tmp);
    }
    else{
      int x_mon = copie2->x;
      int y_mon = copie2->y;
      if (monde[x_mon][y_mon] > 0){
        copie2 -> energie += monde[x_mon][y_mon];
        monde[x_mon][y_mon] = temps_repousse_herbe;
      }
      copie2 = copie2 -> suivant;
    }
  }
  reproduce(liste_proie , p_reproduce_proie);
}

// Cette fonction vérifie la présence d'un animal dans l'environement en position x y
Animal *animal_en_XY(Animal *l, int x, int y) {
  Animal*copie = l;
  while(copie){
    if(((copie -> x) == x) && ((copie -> y) == y)){
      return copie;
    }
    copie = copie -> suivant;
  }
  return NULL;
} 

// On rafraichi les prédateurs
// On déplace les prédateurs et on leur enleve 1 d'énergie 

// On verifie l'emplacement de l'animal : si il y a une proie à son emplacement deux cas sont possible
//---------- Il mange la proie si il a plus d'énergie qu'elle et suivant un proba p_manger et ssi l'option manger_ssi_engergie == 1
//---------- Il mange la proie suivant la proba p_manger et ssi l'option manger_ssi_engergie == 0
// On supprime ceux qui n'ont plu d'energie
// On fait reproduire les prédateurs toujours selon la proba p_reproduce_predateur
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie){
  Animal * copie1 = *liste_predateur;
  Animal * copie2 = *liste_predateur;
  bouger_animaux(copie2);

  while(copie1){
    copie1 -> energie -= 1;
    if(liste_proie){
      Animal*proie1 = animal_en_XY(*liste_proie , copie1->x , copie1->y);

      if(manger_ssi_engergie == 1){
        /*On mange la proie ssi le prédateur à plus d'energie quelle*/
        if ( proie1 != NULL && rand()*1./RAND_MAX < p_manger && (copie1 -> energie >= proie1 -> energie) ){
        copie1 -> energie += proie1 -> energie;
        enlever_animal(liste_proie , proie1);
        }
      }

      /* Si l'option de manger ssi energie est désactivé */
      if(manger_ssi_engergie == 0){
        if (proie1 != NULL && rand()*1./RAND_MAX < p_manger ){
        copie1 -> energie += proie1 -> energie;
        enlever_animal(liste_proie , proie1);
        }
      }
    }
    if(copie1 -> energie < 0){
      Animal *tmp = copie1;
      copie1 = copie1->suivant;
      enlever_animal(liste_predateur , tmp);
    }
    else {
      copie1 = copie1 -> suivant;
    }
  }
  reproduce(liste_predateur, p_reproduce_predateur);
}

// cette fonction permet de rafraichir le monde
// Elle ajoute une quantité d'herbe à chaque case (fais pousser l'herbe un peu plus à chaques étapes de la simulation)
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
  for(int i = 0; i < SIZE_X ; i++){
    for(int j = 0 ; j < SIZE_Y ; j++){
      monde[i][j]+=1;
    }
  }
}


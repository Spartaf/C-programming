#ifndef _ECOSYS_H_
#define _ECOSYS_H_

// Définition des variable gloabale
#define SIZE_X 20
#define SIZE_Y 50

extern float p_ch_dir;
extern float p_reproduce_proie;
extern float p_reproduce_predateur;
extern int temps_repousse_herbe;
extern float p_manger;
extern int manger_ssi_engergie;


typedef struct _animal {
  int x;
  int y;
  int dir[2]; /* direction courante sous la forme (dx, dy) */
  float energie;
  struct _animal *suivant;
} Animal;

Animal *creer_animal(int x, int y, float energie);
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal);
unsigned int compte_animal_rec(Animal *la);
unsigned int compte_animal_it(Animal *la);

void ajouter_animal(int x, int y, float energie, Animal **liste_animal);
void enlever_animal(Animal **liste, Animal *animal);
Animal* liberer_liste_animaux(Animal *liste);
void afficher_ecosys(Animal *liste_predateur, Animal *liste_proie);

void ecrire_ecosys(char *mon_fichier, Animal*liste_predateur, Animal*liste_proie);
void lire_ecosys(char *mon_fichier , Animal **liste_proie , Animal **liste_predateur);
void ecrire_graphique(char* mon_graphique , int iteration , Animal*liste_predateur, Animal*liste_proie);

void reproduce(Animal **liste_animal, float p_reproduce);
void bouger_animaux(Animal *la);
Animal *animal_en_XY(Animal *l, int x, int y);
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie);
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) ;
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]);

void clear_screen();

#endif
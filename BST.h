#include <stdlib.h>
#include <stdio.h>
#define NMAX 30


typedef struct Benevole {
    char nom[NMAX];
    char prenom[NMAX];
    char sexe;
    int CIN;
    int annee;
    struct Benevole * suivant;
} Benevole;

typedef struct ListBenevoles {
    int NbreElements;
    Benevole * Benevole;
} ListBenevoles;

typedef struct Tranche {
    ListBenevoles * ListBenevoles;
    int BorneSup;
    struct Tranche * pere;
    struct Tranche * filsG;
    struct Tranche *filsD;
    int marque;
} Tranche;

int anneeActuelle();

Benevole *nouveauBen(char *nom, char* prenom, int CIN, char sexe, int annee);

Tranche *nouvelleTranche(int borneSup);

ListBenevoles *nouvelleListe();

Tranche *ajoutTranche(Tranche *racine, int BorneSup);

Benevole * chercherBen(Tranche *racine, int CIN, int annee);

int supprimerBen(Tranche *racine, int CIN, int annee);

//STRUCT RAJOUTEES POUR FAIRE UNE PILE
typedef struct pileTranche{ //nouvelle structure pour la fonction actualiser
    int nbElem;
    Tranche** tab;
}pileTranche;

//----------------


//FONCTIONS RAJOUTEES POUR LA PILE
pileTranche* creerPile();
void empiler(pileTranche* p, Tranche* t);
int pileVide(pileTranche* p);
Tranche* depiler(pileTranche* p);
//--------------------


Tranche* minimum(Tranche* racine);

Benevole* insererBen(Tranche *racine, Benevole* benevole);

int supprimerBen(Tranche* racine, int CIN, int annee);

int supprimerTranche(Tranche **racine, int borneSup);                           

int actualiser(Tranche* racine);                                                        

int totalBenTranche(Tranche *racine , int borneSup);

int totalBen(Tranche* racine);

float pourcentageTranche(Tranche *racine , int borneSup);

void afficherTranche(Tranche* racine, int borneSup);

void afficherArbre(Tranche *racine);

ListBenevoles *BendHonneur(Tranche *racine);

void detruireArbre(Tranche * racine);

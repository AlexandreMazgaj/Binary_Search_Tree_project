#include "TP04.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//.......................
//CREATION DES STRUCTURES
//.......................
Benevole *nouveauBen(char *nom, char* prenom, int CIN, char sexe, int annee) {
    Benevole * new = (Benevole *) malloc(sizeof(Benevole));
    strcpy(new->nom, nom);
    strcpy(new->prenom, prenom);
    new->CIN = CIN;
    new->sexe = sexe;
    new->annee = annee;
    new->suivant = NULL;
    return new;
}

Tranche *nouvelleTranche(int borneSup) {
    Tranche * new = (Tranche *) malloc(sizeof(Tranche));
    new->BorneSup = borneSup;
    new->filsD = NULL;
    new->filsG = NULL;
    new->pere = NULL;
    new->ListBenevoles = nouvelleListe();
    new->marque = 0;
    return new;
}

ListBenevoles *nouvelleListe() {
    ListBenevoles * new = (ListBenevoles *) malloc(sizeof(ListBenevoles));
    new->NbreElements = 0;
    new->Benevole = NULL;

    return new;
}

int anneeActuelle(){
    time_t secondes;
    struct tm instant;
    time(&secondes);
    instant = *localtime(&secondes);
    return instant.tm_year + 1900;
}

//..............
//MINIMUM DE ABR
//..............

Tranche* minimum(Tranche* racine){
    Tranche* pTrancheC = racine;
    Tranche* pTrancheP = NULL;
    //on calcul le minimum d'un ABR
    while(pTrancheC != NULL){
        pTrancheP = pTrancheC;
        pTrancheC = pTrancheC->filsG;
    }

    return pTrancheP;
}

//...................
//AJOUT D'UNE TRANCHE
//...................

Tranche *ajoutTranche(Tranche *racine, int BorneSup) {

    if(BorneSup < 20) {
        printf("Borne trop petite\n");
        return NULL;
    }
    if(BorneSup%5 != 0){
        printf("La borne n'est pas un multiple de 5\n");

    }

    Tranche * pCourant = racine;
    while(pCourant != NULL) {
        if(pCourant->BorneSup == BorneSup) {
            printf("Borne deja existante\n");
            return NULL;
        }
        if(pCourant->BorneSup < BorneSup) {
            if(pCourant->filsD)
                pCourant = pCourant->filsD;
            else {
                pCourant->filsD = nouvelleTranche(BorneSup);
                pCourant->filsD->pere = pCourant;
                return pCourant->filsD;
            }
        }
        if(pCourant->BorneSup > BorneSup) {
            if(pCourant->filsG)
                pCourant = pCourant->filsG;
            else {
                pCourant->filsG = nouvelleTranche(BorneSup);
                pCourant->filsG->pere = pCourant;
                return pCourant->filsG;
            }
        }
    }
    if(racine == NULL){
        Tranche* new = nouvelleTranche(BorneSup);
        return new;
    }
    else return NULL;
}

//................
//INSERER BENEVOLE
//................

Benevole* insererBen(Tranche *racine, Benevole *benevole){
    Tranche* pTranche = racine;

    if(racine == NULL || benevole == NULL){
        printf("ERROR NULL\n");
        return NULL;
    }

    while(pTranche != NULL && !((anneeActuelle() - benevole->annee)<= pTranche->BorneSup && (anneeActuelle() - benevole->annee)>=(pTranche->BorneSup-4))){
        if((anneeActuelle() - benevole->annee) > pTranche->BorneSup) pTranche = pTranche->filsD;
        else pTranche = pTranche->filsG;
    }

    if(pTranche != NULL){    //La tranche d'age du benevole existe
        ListBenevoles* pListe = pTranche->ListBenevoles;


        Benevole* pBeneC = pListe->Benevole;
        Benevole* pBeneP = NULL;

        if(pBeneC == NULL){
            pListe->Benevole = benevole;        //benevole est le 1er �l�ment de la liste
            pListe->NbreElements++;

            return benevole;
        }

        while(pBeneC != NULL && (pBeneC->annee >= benevole->annee) && pBeneC->CIN != benevole->CIN){  //On verifie s'il est pas dans la liste
            pBeneP = pBeneC;                                                      //Et on le met dans l'ordre croissant
            pBeneC = pBeneC->suivant;
        }

        if(pBeneC == NULL) {
            pBeneP->suivant = benevole;
            pListe->NbreElements++;

            return benevole;
        }
        else if(pBeneC->CIN == benevole->CIN){           //Le benevole existe deja
            return NULL;
        }
        else if(pBeneP == NULL){
            benevole->suivant = pBeneC;             //le benevole devient le premier element de la liste
            pListe->Benevole = benevole;
            pListe->NbreElements++;

            return benevole;
        }
        else{
            pBeneP->suivant = benevole;
            benevole->suivant = pBeneC;
            pListe->NbreElements++;

            return benevole;
        }

    }
    else{           //la tranche d'age du benevole n'existe pas;
        int borne;
        if((anneeActuelle()-benevole->annee)%5==0)
            borne = (anneeActuelle() - benevole->annee);
        else
            borne = 5*((anneeActuelle() - benevole->annee)/5)+5;

        Tranche* new = ajoutTranche(racine, borne);
        new->ListBenevoles = nouvelleListe();
        new->ListBenevoles->Benevole = benevole;           //On ajoute la nouvelle tranche et on met le benevole dans cette nouvelle tranche
        new->ListBenevoles->NbreElements++;

        return new->ListBenevoles->Benevole;
    }

}


//.................
//CHERCHER BENEVOLE
//.................

Benevole * chercherBen(Tranche *racine, int CIN, int annee) {
    if(!racine) {
        printf("Racine inexistante");
        return NULL;
    }
    if(CIN < 0 || annee < 0) {
        printf("Parametres de recherche incorrectes");
        return NULL;
    }

    Tranche * pCourant = racine;
    while (pCourant != NULL && ((pCourant->BorneSup < (anneeActuelle()-annee)) || ((pCourant->BorneSup-4) > (anneeActuelle()-annee)))) {
        if(anneeActuelle()-annee < pCourant->BorneSup)
            pCourant = pCourant->filsG;
        else
            pCourant = pCourant->filsD;
    }

    if(!pCourant) {
        printf("Tranche correspondant a l'annee inexistante");
        return NULL;
    }

    Benevole * pBene = pCourant->ListBenevoles->Benevole;
    while(!pBene) {
        if(pBene->CIN == CIN)
            return pBene;
        pBene = pBene->suivant;
    }
    printf("Benevole inexistant sur cette annee");
    return NULL;
}



//.....................
//SUPPRIMER UN BENEVOLE
//.....................

int supprimerBen(Tranche *racine, int CIN, int annee) {
    if(!racine) {
        printf("Racine inexistante\n");
        return 0;
    }
    if(CIN < 0 || annee < 0) {
        printf("Parametres de recherche incorrectes\n");
        return 0;
    }

    Tranche * pCourant = racine;
    while (pCourant != NULL && ((pCourant->BorneSup < (anneeActuelle()-annee)) || ((pCourant->BorneSup-4) > (anneeActuelle()-annee)))) {
        if(anneeActuelle()-annee < pCourant->BorneSup)
            pCourant = pCourant->filsG;
        else
            pCourant = pCourant->filsD;
    }
    if(!pCourant) {
        printf("Tranche introuvable\n");
        return 0;
    }
    Benevole * pBene = pCourant->ListBenevoles->Benevole;
    Benevole * pBenePrec = NULL;
    while(pBene !=NULL && pBene->CIN != CIN) {
        pBenePrec = pBene;
        pBene = pBene->suivant;
    }
    if(!pBene) {
        printf("Benevole introuvable\n");
        return 0;
    }

    if(pBene->CIN == CIN){
        if(pBenePrec == NULL){
            pCourant->ListBenevoles->NbreElements--;            //Il n'y a qu'un seul element dans la liste
            pCourant->ListBenevoles->Benevole = NULL;
        }
        else{
            pBenePrec->suivant = pBene->suivant;
            pCourant->ListBenevoles->NbreElements--;
        }
    }
    else{
        pCourant->ListBenevoles->Benevole = pBene->suivant;
        pCourant->ListBenevoles->NbreElements--;
    }

    if(pCourant->ListBenevoles->NbreElements == 0){
        supprimerTranche(&racine, pCourant->BorneSup);
    }

    free(pBene);
    return 1;
}



//.................
//SUPPRIMER TRANCHE
//.................

int supprimerTranche(Tranche **racine, int borneSup){
    Tranche* pTrancheC = *racine;    //on va chercher la tranche � supprimer
    Tranche* pTrancheP = NULL;
    if(racine == NULL || borneSup <0)
        return 1;

    while(pTrancheC != NULL && !(borneSup<= pTrancheC->BorneSup && borneSup>=(pTrancheC->BorneSup-4))){
        if(pTrancheC->BorneSup > borneSup){
            pTrancheP = pTrancheC;
            pTrancheC = pTrancheC->filsG;       //On cherche le noeud � supprimer
        }
        else{
            pTrancheP = pTrancheC;
            pTrancheC = pTrancheC->filsD;

        }
    }
    if(pTrancheC == NULL){
        printf("Tranche que vous voulez retourner n'existe pas\n");
        return 1;
    }
    else{
        if(pTrancheC->filsD== NULL && pTrancheC->filsG==NULL){          //l'element a supprimer n'a pas de fils
            Benevole* pBen = pTrancheC->ListBenevoles->Benevole;
            if(pBen == NULL){
                free(pBen);

            }
            else{
                Benevole* pBenS = pBen->suivant;
                free(pBen);
                while(pBenS != NULL){
                    pBen = pBenS;                               //Supprime tous les Benevoles
                    pBenS = pBen->suivant;
                    free(pBen);
                }
            }
            if(pTrancheC->pere->filsD == pTrancheC)
                pTrancheC->pere->filsD = NULL;
            else
                pTrancheC->pere->filsG = NULL;

            free(pTrancheC);
            return 0;
        }
        else if(pTrancheC->filsD== NULL || pTrancheC->filsG==NULL){
            if(pTrancheC->filsD != NULL){
                if(!pTrancheP) {
                    *racine = pTrancheC->filsD;
                }
                else if(pTrancheP->filsD == pTrancheC)            //l'�l�ment � supprimer � un seul fils
                    pTrancheP->filsD = pTrancheC->filsD;
                else
                    pTrancheP->filsG = pTrancheC->filsD;        //On verifie si le noeud � supprimer � son fils a gauche ou a droite
            }
            else{
                if(!pTrancheP) {
                    *racine = pTrancheC->filsG;
                }
                else if(pTrancheP->filsD == pTrancheC)
                    pTrancheP->filsD = pTrancheC->filsG;
                else
                    pTrancheP->filsG = pTrancheC->filsG;
            }
            Benevole* pBen = pTrancheC->ListBenevoles->Benevole;
            if(pBen == NULL){
                free(pBen);
            }
            else{
                Benevole* pBenS = pBen->suivant;
                free(pBen);
                while(pBenS != NULL){
                    pBen = pBenS;                               //Supprime tous les Benevoles
                    pBenS = pBen->suivant;
                    free(pBen);
                }
            }

            free(pTrancheC);

            return 0;
        }
        else{
            Tranche* succ = minimum(pTrancheC->filsD);            //On recherche le successeurs du noeud
            printf("%d\n",succ->BorneSup);
            pTrancheC->BorneSup = succ->BorneSup;                //l'�l�ment � supprimer � deux fils
            printf("%d\n",pTrancheC->BorneSup);
            pTrancheC->ListBenevoles = succ->ListBenevoles;   //On change de place le succ et le noeud

            int n = supprimerTranche(&succ, succ->BorneSup);     //on rappel la fonction pour le supprimer
            return 0;

        }
    }

}



//.............................
//TROUVER LE BENEVOLE D'HONNEUR
//.............................

ListBenevoles *BendHonneur(Tranche *racine) {
    if(!racine) {
        printf("Tranche NULL\n");
        return NULL;
    }

    Tranche * pCourant = racine;
    while(pCourant->filsD) {
        pCourant = pCourant->filsD;
    }

    Benevole * pBene = pCourant->ListBenevoles->Benevole;

    int anneeNaissance = anneeActuelle() - pCourant->BorneSup + 4;
    while(pBene) {
        if(pBene->annee < anneeNaissance)
            anneeNaissance = pBene->annee;
        pBene = pBene->suivant;
    }

    pBene = pCourant->ListBenevoles->Benevole;
    Benevole * pBeneN = NULL;
    ListBenevoles * newList = nouvelleListe();

    while(pBene) {
        if(anneeNaissance == pBene->annee) {
            if(pBeneN) {
                pBeneN->suivant = nouveauBen(pBene->nom, pBene->prenom, pBene->CIN, pBene->sexe, pBene->annee);
                pBeneN = pBeneN->suivant;
                newList->NbreElements++;
            }
            else {
                pBeneN = nouveauBen(pBene->nom, pBene->prenom, pBene->CIN, pBene->sexe, pBene->annee);
                newList->Benevole = pBeneN;
                newList->NbreElements++;
            }
        }
        pBene = pBene->suivant;
    }
    return newList;
}



//......................
//FONCTIONS POUR LA PILE
//......................
pileTranche* creerPile(){
    pileTranche* p = (pileTranche*)malloc(sizeof(pileTranche));
    p->tab = (Tranche **)malloc(NMAX*sizeof(Tranche*));
    p->nbElem = 0;

    return p;
}

Tranche* sommet(pileTranche *p) {
    return p->tab[p->nbElem];
}

void empiler(pileTranche* p, Tranche* t){
    p->nbElem++;
    p->tab[p->nbElem] = t;
}

int pileVide(pileTranche* p){
    if(p->nbElem == 0) return 1;
    else return 0;
}

Tranche* depiler(pileTranche* p){
    if(!(pileVide(p))){
        Tranche* nc = p->tab[p->nbElem];
        p->nbElem--;

        return nc;
    }
    else return NULL;
}

void marquer(Tranche * t) {
    t->marque = 1;
}

int elementMarque(Tranche * t) {
    return t->marque;
}



//.................
//ACTUALISER UN ABR
//.................

int actualiser(Tranche* racine){

    int n = 0;
    Benevole* pBeneC = NULL;                    //pointeur avec lequel on va parcourir les listes de benevoles
    Benevole* pBeneP = NULL;
    Benevole* pBeneS = NULL;

    Tranche* pCourant = racine;
    pileTranche* p = creerPile();               //pile dans laquelle on va empiler le tranches � parcourir

    empiler(p, pCourant);

    while(!pileVide(p)){
        pCourant = depiler(p);
        if(pCourant->filsD != NULL) empiler(p, pCourant->filsD);        //on va parcourir toutes les tranches de l'arbre
        if(pCourant->filsG != NULL) empiler(p, pCourant->filsG);

        pBeneC = pCourant->ListBenevoles->Benevole;
        pBeneP = NULL;
        pBeneS = NULL;

        while(pBeneC != NULL){
            Benevole* temp = NULL;
            if((anneeActuelle() - pBeneC->annee)> pCourant->BorneSup){          //Si le benvole n'appartient plus � sa tranche actuelle
                if(pBeneP == NULL){
                    pCourant->ListBenevoles->Benevole = pBeneC->suivant;       //on fait en sorte de ne pas d�ranger la liste de la tranche
                }
                else pBeneP->suivant = pBeneC->suivant;

                n++;
                pCourant->ListBenevoles->NbreElements--;
                temp = insererBen(racine, pBeneC);
            }
            pBeneS = pBeneC->suivant;
                           //On insere le benevole dans sa nouvelle tranche
            if(temp == NULL)
                pBeneP = pBeneC;
            pBeneC = pBeneS;
        }

        if(pCourant->ListBenevoles->NbreElements <= 0)
            printf("%d", pCourant->BorneSup);
            supprimerTranche(&racine, pCourant->BorneSup);

    }


    free(p);

    return n;
}



//...................................
//NBRE TOTAL DE BENVOLE D'UNE TRANCHE
//...................................

int totalBenTranche(Tranche *racine, int borneSup) {
    if(!racine) {
        printf("Racine inexistante");
        return 0;
    }
    if(borneSup < 20) {
        printf("Borne Sup trop petite");
        return 0;
    }

    Tranche * pCourant = racine;
    while (pCourant != NULL && pCourant->BorneSup != borneSup) {
        if(borneSup < pCourant->BorneSup)
            pCourant = pCourant->filsG;
        else
            pCourant = pCourant->filsD;
    }
    if(pCourant)
        return pCourant->ListBenevoles->NbreElements;
    else
        return 0;
}



//......................
//NBRE TOTAL DE BENEVOLE
//......................

int totalBen(Tranche* racine){
    int n =0;
    Tranche* pCourant = racine;
    pileTranche* p = creerPile();

    empiler(p, pCourant);

    while(!(pileVide(p))){
        pCourant = depiler(p);
        if(pCourant->filsD != NULL) empiler(p, pCourant->filsD);        //on va parcourir toutes les tranches de l'arbre
        if(pCourant->filsG != NULL) empiler(p, pCourant->filsG);

        n = n + pCourant->ListBenevoles->NbreElements;
    }

    return n;
}



//.......................
//POURCENTAGE DE BENEVOLE
//.......................

float pourcentageTranche(Tranche * racine, int borneSup) {
    return 100.0 * totalBenTranche(racine, borneSup)/totalBen(racine);
}



//....................................
//AFFICHER LES BENEVOLES D'UNE TRANCHE
//....................................

void afficherTranche(Tranche* racine, int borneSup){

    Tranche* pTrancheC = racine;    //on va chercher la tranche ‡ supprimer

    if(racine == NULL || borneSup <0)
        return ;
    while(pTrancheC != NULL && !(borneSup<= pTrancheC->BorneSup && borneSup>=(pTrancheC->BorneSup-4))){
        if(pTrancheC->BorneSup > borneSup){
            pTrancheC = pTrancheC->filsG;       //On cherche la tranche dont on doit afficher les benevoles
        }
        else{
            pTrancheC = pTrancheC->filsD;

        }
    }

    if(pTrancheC == NULL){
        printf("la tranche que vous voulez afficher n'existe pas\n");
        return;
    }
    else{
        Benevole* pBene = pTrancheC->ListBenevoles->Benevole;

        printf("Tranche des %d - %d ans\n", borneSup-4, borneSup);
        printf("---------------------------\n");
        printf("---------------------------\n");

        int i = 1;
        while(pBene !=NULL){
            printf("Benevole %d : \n", i);
            printf("------------\n");
            printf("nom : %s\n", pBene->nom);
            printf("prenom : %s\n", pBene->prenom);
            printf("annee de naissance : %d\n", pBene->annee);
            printf("sexe : %c\n", pBene->sexe);
            printf("numero de carte d'identite : %d\n", pBene->CIN);
            printf("\n");
            i++;

            pBene = pBene->suivant;
        }

    }
}



//................
//AFFICHER L'ARBRE
//................



void afficherArbre(Tranche *racine) {
    if(!racine) {
        printf("Racine NULL");
        return;
    }
    printf("|---------|---------------------|\n");
    printf("| Tranche | Nombre de benevoles |\n");
    printf("|---------|---------------------|\n");
    pileTranche *p = creerPile();
    Tranche * nc;
    empiler(p, racine);
    while(!pileVide(p)) {
        nc = sommet(p);
        if(nc->filsG && !elementMarque(nc->filsG)) {
            empiler(p, nc->filsG);
        }
        else {
            printf("| %-7d | %-19d |\n", nc->BorneSup, nc->ListBenevoles->NbreElements);
            printf("|---------|---------------------|\n");
            marquer(nc);
            depiler(p);
            if(nc->filsD) {
                empiler(p, nc->filsD);
            }
        }
    }

    //suppression du marquage une fois l'affichage effectué

    empiler(p, racine);
    while(!pileVide(p)) {
        nc = depiler(p);
        if(nc->filsD)
            empiler(p, nc->filsD);
        if(nc->filsG)
            empiler(p, nc->filsG);
        nc->marque = 0;
    }
}

void detruireArbre(Tranche * racine) {
    pileTranche * p = creerPile();

    empiler(p, racine);
    Tranche * nc;
    while(!pileVide(p)) {
        nc = depiler(p);
        if(nc->filsG)
            empiler(p, nc->filsG);
        if(nc->filsD)
            empiler(p, nc->filsD);
        free(nc);
    }
}

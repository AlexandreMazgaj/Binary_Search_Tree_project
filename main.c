#include <stdio.h>
#include <stdlib.h>
#include "TP04.h"

int main(int argc, const char * argv[]) {

    char menu[] =   "|---------------------------------------------|\n"
                    "|            Menu de l'application            |\n"
                    "|---------------------------------------------|\n"
                    "|1.  Initialiser un arbre                     |\n"
                    "|2.  Ajouter une tranche                      |\n"
                    "|3.  Ajouter un benevole                      |\n"
                    "|4.  Afficher les tranches d'age              |\n"
                    "|5.  Afficher les benevoles d'une tranche     |\n"
                    "|6.  Supprimer un benevole                    |\n"
                    "|7.  Supprimer une tranche                    |\n"
                    "|8.  Afficher les benevoles d'honneur         |\n"
                    "|9.  Afficher le nombre total de benevoles    |\n"
                    "|10. Actualiser l'arbre                       |\n"
                    "|11. Afficher le pourcentage de benevoles     |\n"
                    "|    d'une tranche                            |\n"
                    "|12. Detruire l'arbre                         |\n"
                    "|13. Quitter                                  |\n"
                    "|---------------------------------------------|\n"
                    "Votre choix ?\n";

    int choix = 0;
    Tranche * racine;

    while(choix != 13) {
        printf("%s", menu);
        scanf("%d",&choix);
        //system("cls");


        switch(choix) {
            case 1:
                racine = nouvelleTranche(30);
                if(racine)
                    printf("Arbre initialise\n");
                break;
            case 2: {
                int borneSup;
                printf("Borne superieur ?");
                scanf("%d", &borneSup);
                ajoutTranche(racine,borneSup);
                break;
            }
            case 3: {
                char nom[NMAX];
                char prenom[NMAX];
                int CIN;
                char sexe;
                int annee;

                printf("Nom du benevole :\n");
                scanf("%s", &nom);
                printf("Prenom du benevole :\n");
                scanf("%s", &prenom);
                printf("CIN :\n");
                scanf("%d", &CIN);
                printf("Sexe :\n");
                scanf(" %c", &sexe);
                printf("Annee de naissance :\n");
                scanf("%d", &annee);

                Benevole * ben = nouveauBen(nom, prenom, CIN, sexe, annee);
                insererBen(racine,ben);
                break;
            }
            case 4: {
                afficherArbre(racine);
                break;
            }
            case 5: {
                int borneSup;
                printf("Quelle tranche afficher ?\n");
                scanf("%d", &borneSup);
                afficherTranche(racine, borneSup);
                break;
            }
            case 6: {
                int annee, CIN;
                printf("Annee de naissance du benevole :\n");
                scanf("%d", &annee);
                printf("CIN :\n");
                scanf("%d", &CIN);
                supprimerBen(racine, CIN, annee);
                break;
            }
            case 7: {
                int borneSup;
                printf("Quelle tranche supprimer ?\n");
                scanf("%d", &borneSup);
                supprimerTranche(&racine, borneSup);
                break;
            }
            case 8: {
                ListBenevoles * lBene = BendHonneur(racine);
                if(!lBene->NbreElements) {
                    printf("Pas de Benevoles d'Honneur");
                    break;
                }
                Benevole * pCourant = lBene->Benevole;
                int i = 1;
                while(pCourant) {
                    printf("Benevole %d : \n", i);
                    printf("------------\n");
                    printf("Nom : %s\n", pCourant->nom);
                    printf("Prenom : %s\n", pCourant->prenom);
                    printf("Annee de naissance : %d\n", pCourant->annee);
                    printf("Sexe : %c\n", pCourant->sexe);
                    printf("Numero de carte d'identite : %d\n", pCourant->CIN);
                    printf("\n");
                    i++;
                    pCourant = pCourant->suivant;
                }
                break;
            }
            case 9: {
                printf("%d", totalBen(racine));
                break;
            }
            case 10: {
                actualiser(racine);
                break;
            }
            case 11: {
                int borneSup;
                printf("Quelle tranche afficher ?");
                scanf("%d", &borneSup);
                printf("Pourcentage : %f", pourcentageTranche(racine, borneSup));
                break;
            }
            case 12: {
                detruireArbre(racine);
                racine = NULL;
                break;
            }
        }
        printf("\n");
    }

    return 0;
}

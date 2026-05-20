#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// Déclaration des fonctions situées dans tes autres fichiers
void modeEnseignant();
void modeEtudiant();

int main() {
    int choix = 0;

    printf("=========================================\n");
    printf("       GESTIONNAIRE DE QCM - CY TECH     \n");
    printf("=========================================\n");

    while (choix != 3) {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Mode Enseignant\n");
        printf("2. Mode Etudiant\n");
        printf("3. Quitter l'application\n");
        printf("Votre choix : ");

        // Lecture sécurisée du choix utilisateur
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n'); // Nettoyage du buffer si l'utilisateur tape une lettre
            printf("Saisie invalide. Veuillez entrer un nombre.\n");
            continue;
        }

        switch (choix) {
            case 1:
                modeEnseignant();
                break;
            case 2:
                modeEtudiant();
                break;
            case 3:
                printf("Fermeture de l'application. A bientot !\n");
                break;
            default:
                printf("Choix invalide. Veuillez entrer 1, 2 ou 3.\n");
        }
    }

    return 0;
}
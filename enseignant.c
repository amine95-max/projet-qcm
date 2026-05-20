#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// Variable globale pour gérer le mot de passe modifiable
char motDePasseActuel[50] = MOT_DE_PASSE_DEFAUT;

/* -------------------------------------------------------------------------
   Fonctions utilitaires (sans utiliser string.h)
   ------------------------------------------------------------------------- */
int comparerChaines(const char *chaine1, const char *chaine2) {
    int i = 0;
    while (chaine1[i] != '\0' && chaine2[i] != '\0') {
        if (chaine1[i] != chaine2[i]) {
            return chaine1[i] - chaine2[i];
        }
        i++;
    }
    return chaine1[i] - chaine2[i];
}

void copierChaine(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void nettoyerSaisieEnseignant(char *chaine) {
    int i = 0;
    while (chaine[i] != '\0') {
        if (chaine[i] == '\n') {
            chaine[i] = '\0';
            break;
        }
        i++;
    }
}

// ---------------------------------------------
// Fonction : Modifier le mot de passe
// ---------------------------------------------
void modifierMotDePasse() {
    char nouveau[50], confirmation[50];
    printf("\n--- MODIFICATION DU MOT DE PASSE ---\n");
    printf("Entrez le nouveau mot de passe : ");
    scanf("%49s", nouveau);
    printf("Confirmez le nouveau mot de passe : ");
    scanf("%49s", confirmation);

    if (comparerChaines(nouveau, confirmation) == 0) {
        copierChaine(motDePasseActuel, nouveau);
        printf("Mot de passe mis à jour avec succès.\n");
    } else {
        printf("Erreur : les mots de passe ne correspondent pas.\n");
    }
}

// ---------------------------------------------
// Fonction : Authentification
// ---------------------------------------------
int connexionEnseignant() {
    char saisie[50];
    printf("\n--- AUTHENTIFICATION ---\n");
    printf("Entrez le mot de passe enseignant : ");
    scanf("%49s", saisie);

    if (comparerChaines(saisie, motDePasseActuel) == 0) {
        printf("Accès autorisé.\n");
        return 1;
    }
    printf("Accès refusé.\n");
    return 0;
}

// ---------------------------------------------
// Fonction : Créer un QCM (avec vérifications)
// ---------------------------------------------
void creerQCM(QCM *qcm) {
    printf("\n=== CRÉATION D'UN NOUVEAU QCM ===\n");
    printf("Nom du QCM (sans espaces) : ");
    scanf("%255s", qcm->nom);

    do {
        printf("Autoriser les points négatifs ? (1=oui, 0=non) : ");
        if (scanf("%d", &qcm->autoriserNegatif) != 1 || (qcm->autoriserNegatif != 0 && qcm->autoriserNegatif != 1)) {
            printf("Erreur : Entrez 0 ou 1.\n");
            while (getchar() != '\n');
        } else break;
    } while (1);

    do {
        printf("Autoriser plusieurs bonnes réponses ? (1=oui, 0=non) : ");
        if (scanf("%d", &qcm->reponsesMultiples) != 1 || (qcm->reponsesMultiples != 0 && qcm->reponsesMultiples != 1)) {
            printf("Erreur : Entrez 0 ou 1.\n");
            while (getchar() != '\n');
        } else break;
    } while (1);

    do {
        printf("Mode séquentiel ? (1=oui, 0=non) : ");
        if (scanf("%d", &qcm->modeSequentiel) != 1 || (qcm->modeSequentiel != 0 && qcm->modeSequentiel != 1)) {
            printf("Erreur : Entrez 0 ou 1.\n");
            while (getchar() != '\n');
        } else break;
    } while (1);

    do {
        printf("Nombre de questions (Max %d) : ", MAX_QUESTIONS);
        if (scanf("%d", &qcm->nbQuestions) != 1 || qcm->nbQuestions <= 0 || qcm->nbQuestions > MAX_QUESTIONS) {
            printf("Erreur : Nombre invalide.\n");
            while (getchar() != '\n');
        } else break;
    } while (1);

    for (int i = 0; i < qcm->nbQuestions; i++) {
        printf("\n--- Question %d ---\n", i + 1);
        printf("Texte de la question : ");
        while (getchar() != '\n'); 
        fgets(qcm->questions[i].texteQuestion, MAX_TEXTE, stdin);
        nettoyerSaisieEnseignant(qcm->questions[i].texteQuestion);

        do {
            printf("Nombre de choix (Max %d) : ", MAX_CHOIX);
            if (scanf("%d", &qcm->questions[i].nbChoix) != 1 || qcm->questions[i].nbChoix <= 1 || qcm->questions[i].nbChoix > MAX_CHOIX) {
                printf("Erreur : Minimum 2 choix.\n");
                while (getchar() != '\n');
            } else break;
        } while (1);

        int auMoinsUneBonne = 0;
        for (int j = 0; j < qcm->questions[i].nbChoix; j++) {
            printf("  Texte du choix %d : ", j + 1);
            while (getchar() != '\n');
            fgets(qcm->questions[i].choix[j].texte, MAX_TEXTE, stdin);
            nettoyerSaisieEnseignant(qcm->questions[i].choix[j].texte);

            do {
                printf("  Correct ? (1=oui, 0=non) : ");
                if (scanf("%d", &qcm->questions[i].choix[j].estCorrect) != 1) {
                    while (getchar() != '\n');
                } else break;
            } while (1);
            if (qcm->questions[i].choix[j].estCorrect) auMoinsUneBonne++;
        }
        if (auMoinsUneBonne == 0) printf("! ATTENTION : Aucune bonne réponse définie.\n");
    }
}

// ---------------------------------------------
// Fonction : Sauvegarder un QCM
// ---------------------------------------------
void sauvegarderQCM(QCM qcm) {
    char nomFichier[300];
    sprintf(nomFichier, "%s.qcm", qcm.nom);
    FILE *f = fopen(nomFichier, "w");
    if (!f) return;

    fprintf(f, "%s\n%d %d %d\n%d\n", qcm.nom, qcm.autoriserNegatif, qcm.reponsesMultiples, qcm.modeSequentiel, qcm.nbQuestions);
    for (int i = 0; i < qcm.nbQuestions; i++) {
        fprintf(f, "%s\n%d\n", qcm.questions[i].texteQuestion, qcm.questions[i].nbChoix);
        for (int j = 0; j < qcm.questions[i].nbChoix; j++) {
            fprintf(f, "%s\n%d\n", qcm.questions[i].choix[j].texte, qcm.questions[i].choix[j].estCorrect);
        }
    }
    fclose(f);
    printf("\nQCM sauvegardé : %s\n", nomFichier);
}

// ---------------------------------------------
// Fonction : Mode Enseignant (Menu Principal)
// ---------------------------------------------
void modeEnseignant() {
    if (!connexionEnseignant()) return;

    int choix = 0;
    QCM monQcm;

    while (choix != 3) {
        printf("\n--- MENU ENSEIGNANT ---\n");
        printf("1. Créer un nouveau QCM\n");
        printf("2. Modifier le mot de passe\n");
        printf("3. Quitter le mode enseignant\n");
        printf("Votre choix : ");
        
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choix) {
            case 1:
                creerQCM(&monQcm);
                sauvegarderQCM(monQcm);
                break;
            case 2:
                modifierMotDePasse();
                break;
            case 3:
                printf("Déconnexion...\n");
                break;
            default:
                printf("Option invalide.\n");
        }
    }
}
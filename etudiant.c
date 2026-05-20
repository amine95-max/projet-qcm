#include <stdio.h>
#include <stdlib.h>
#include "structures.h" 

/* -------------------------------------------------------------------------
   Fonctions utilitaires (sans utiliser string.h)
   ------------------------------------------------------------------------- */

// Remplace strcspn pour enlever le '\n' d'une chaîne
void nettoyerSaisie(char *chaine) {
    int i = 0;
    while (chaine[i] != '\0') {
        if (chaine[i] == '\n') {
            chaine[i] = '\0';
            break;
        }
        i++;
    }
}

// Concatène ".qcm" au nom saisi par l'étudiant
void creerNomFichier(const char *nomQCM, char *nomFichier) {
    int i = 0;
    while (nomQCM[i] != '\0') {
        nomFichier[i] = nomQCM[i];
        i++;
    }
    nomFichier[i++] = '.';
    nomFichier[i++] = 'q';
    nomFichier[i++] = 'c';
    nomFichier[i++] = 'm';
    nomFichier[i] = '\0';
}

/* -------------------------------------------------------------------------
   Fonction : Charger un QCM depuis son fichier
   ------------------------------------------------------------------------- */
int chargerQCM(const char *nomQCM, QCM *qcm) {
    char nomFichier[300];
    creerNomFichier(nomQCM, nomFichier);
    
    FILE *f = fopen(nomFichier, "r");
    if (!f) {
        printf("Erreur : Impossible de trouver le QCM '%s'.\n", nomFichier);
        return 0;
    }

    if (fscanf(f, "%255s", qcm->nom) != 1) { fclose(f); return 0; }
    if (fscanf(f, "%d %d %d", &qcm->autoriserNegatif, &qcm->reponsesMultiples, &qcm->modeSequentiel) != 3) { fclose(f); return 0; }
    if (fscanf(f, "%d", &qcm->nbQuestions) != 1) { fclose(f); return 0; }
    fgetc(f); 

    for (int i = 0; i < qcm->nbQuestions; i++) {
        fgets(qcm->questions[i].texteQuestion, MAX_TEXTE, f);
        nettoyerSaisie(qcm->questions[i].texteQuestion);

        if (fscanf(f, "%d", &qcm->questions[i].nbChoix) != 1) { fclose(f); return 0; }
        fgetc(f); 

        for (int j = 0; j < qcm->questions[i].nbChoix; j++) {
            fgets(qcm->questions[i].choix[j].texte, MAX_TEXTE, f);
            nettoyerSaisie(qcm->questions[i].choix[j].texte);

            if (fscanf(f, "%d", &qcm->questions[i].choix[j].estCorrect) != 1) { fclose(f); return 0; }
            fgetc(f); 
        }
    }

    fclose(f);
    return 1;
}

/* -------------------------------------------------------------------------
   Fonction : Passer un QCM
   ------------------------------------------------------------------------- */
void passerQCM(QCM qcm) {
    printf("\n=========================================\n");
    printf("   LANCEMENT DU QCM : %s\n", qcm.nom);
    printf("=========================================\n");
    
    float totalPointsObtenus = 0.0;
    float pointsParQuestion = 20.0 / qcm.nbQuestions; 

    for (int i = 0; i < qcm.nbQuestions; i++) {
        printf("\n--- Question %d/%d ---\n", i + 1, qcm.nbQuestions);
        printf("%s\n", qcm.questions[i].texteQuestion);

        for (int j = 0; j < qcm.questions[i].nbChoix; j++) {
            printf("  %d. %s\n", j + 1, qcm.questions[i].choix[j].texte);
        }

        int reponsesEtudiant[MAX_CHOIX] = {0};
        int saisie = -1;

        if (qcm.reponsesMultiples) {
            printf("\n(Plusieurs reponses possibles. Entrez les numeros un par un, puis 0 pour valider)\n");
            if (!qcm.modeSequentiel) {
                printf("(Entrez -1 pour passer la question sans repondre)\n");
            }
            
            int passerQuestion = 0;
            do {
                printf("Votre choix (0 pour finir) : ");
                if (scanf("%d", &saisie) != 1) {
                    while (getchar() != '\n'); 
                    printf("Saisie invalide.\n");
                    continue;
                }

                if (saisie == -1 && !qcm.modeSequentiel) {
                    passerQuestion = 1;
                    break;
                }

                if (saisie > 0 && saisie <= qcm.questions[i].nbChoix) {
                    reponsesEtudiant[saisie - 1] = !reponsesEtudiant[saisie - 1];
                    printf("  -> Choix %d pris en compte.\n", saisie);
                } else if (saisie != 0) {
                    printf("Numero invalide.\n");
                }
            } while (saisie != 0);

            if (passerQuestion) {
                printf("Question passee.\n");
                continue; 
            }

        } else {
            do {
                if (!qcm.modeSequentiel) {
                    printf("\nVotre choix (ou -1 pour passer) : ");
                } else {
                    printf("\nVotre choix : ");
                }

                if (scanf("%d", &saisie) != 1) {
                    while (getchar() != '\n');
                    printf("Saisie invalide.\n");
                    continue;
                }

                if (saisie == -1 && !qcm.modeSequentiel) {
                    break;
                }

                if (saisie > 0 && saisie <= qcm.questions[i].nbChoix) {
                    reponsesEtudiant[saisie - 1] = 1;
                    break;
                } else {
                    printf("Numero invalide.\n");
                }
            } while (1);

            if (saisie == -1) {
                printf("Question passee.\n");
                continue;
            }
        }

        // --- CALCUL DU SCORE ---
        int fautes = 0;
        int nbReponsesAttendues = 0;
        int nbBonnesReponsesTrouvees = 0;

        for (int j = 0; j < qcm.questions[i].nbChoix; j++) {
            if (qcm.questions[i].choix[j].estCorrect) nbReponsesAttendues++;
            
            if (reponsesEtudiant[j] != qcm.questions[i].choix[j].estCorrect) {
                fautes++;
            } else if (reponsesEtudiant[j] == 1 && qcm.questions[i].choix[j].estCorrect == 1) {
                nbBonnesReponsesTrouvees++;
            }
        }

        if (fautes == 0) {
            totalPointsObtenus += pointsParQuestion;
        } else if (qcm.autoriserNegatif) {
            // Retire des points proportionnellement aux erreurs
            float penalite = ((float)fautes / qcm.questions[i].nbChoix) * pointsParQuestion;
            totalPointsObtenus -= penalite;
        }
    }

    if (totalPointsObtenus < 0) {
        totalPointsObtenus = 0;
    }

    printf("\n=========================================\n");
    printf("   FIN DU QCM. VOTRE NOTE : %.2f / 20.00\n", totalPointsObtenus);
    printf("=========================================\n");
}

/* -------------------------------------------------------------------------
   Fonction : Mode Étudiant (Menu Principal)
   ------------------------------------------------------------------------- */
void modeEtudiant() {
    int choix = 0;
    char nomQCM[MAX_TEXTE];
    QCM qcmEnCours;

    while (choix != 2) {
        printf("\n--- MENU ETUDIANT ---\n");
        printf("1. Passer un QCM\n");
        printf("2. Quitter le mode etudiant\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choix) {
            case 1:
                printf("\nEntrez le nom du QCM a charger (ex: dbz, info_cc1...) : ");
                scanf("%255s", nomQCM);
                
                if (chargerQCM(nomQCM, &qcmEnCours)) {
                    passerQCM(qcmEnCours);
                }
                break;
            case 2:
                printf("Retour au menu...\n");
                break;
            default:
                printf("Option invalide.\n");
        }
    }
}
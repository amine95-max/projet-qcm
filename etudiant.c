#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include "structures.h" 

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

void creerNomFichier(const char *nomQCM, char *nomFichier) {
    int i = 0;
    while (nomQCM[i] != '\0') {
        nomFichier[i] = nomQCM[i];
        i++;
    }
    nomFichier[i++] = '.'; nomFichier[i++] = 'q'; nomFichier[i++] = 'c'; nomFichier[i++] = 'm';
    nomFichier[i] = '\0';
}

int longueurChaine(const char *chaine) {
    int l = 0;
    while(chaine[l] != '\0') l++;
    return l;
}

int estUnFichierQCM(const char *nomFichier) {
    int len = longueurChaine(nomFichier);
    if (len < 4) return 0;
    if (nomFichier[len-4] == '.' && nomFichier[len-3] == 'q' && 
        nomFichier[len-2] == 'c' && nomFichier[len-1] == 'm') {
        return 1;
    }
    return 0;
}

/* -------------------------------------------------------------------------
   LA FAMEUSE FONCTION DU MENU NUMÉROTÉ
   ------------------------------------------------------------------------- */
int choisirQCM(char *nomChoisi) {
    struct dirent *entree;
    DIR *dossier = opendir(".");
    char liste[50][MAX_TEXTE]; 
    int nbFichiers = 0;

    if (dossier == NULL) {
        printf("Erreur : Impossible de lire le dossier actuel.\n");
        return 0; 
    }

    printf("\n=== LISTE DES QCM DISPONIBLES ===\n");

    while ((entree = readdir(dossier)) != NULL) {
        if (estUnFichierQCM(entree->d_name)) {
            int len = longueurChaine(entree->d_name);
            int i;
            for(i = 0; i < len - 4; i++) {
                liste[nbFichiers][i] = entree->d_name[i];
            }
            liste[nbFichiers][i] = '\0';
            
            // Affiche " 1. nom_du_qcm "
            printf("  %d. %s\n", nbFichiers + 1, liste[nbFichiers]);
            nbFichiers++;
            
            if (nbFichiers >= 50) break; 
        }
    }
    closedir(dossier);

    if (nbFichiers == 0) {
        printf("  (Aucun QCM trouve. Creez-en un dans le menu Enseignant !)\n");
        return 0; 
    }

    int choix = 0;
    do {
        // Demande de taper le numéro
        printf("\nEntrez le numero du QCM a passer (1 a %d) ou 0 pour annuler : ", nbFichiers);
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n'); 
            printf("Saisie invalide.\n");
            choix = -1;
        } else if (choix == 0) {
            return 0; 
        } else if (choix < 1 || choix > nbFichiers) {
            printf("Numero invalide.\n");
        }
    } while (choix < 1 || choix > nbFichiers);

    int j = 0;
    while (liste[choix - 1][j] != '\0') {
        nomChoisi[j] = liste[choix - 1][j];
        j++;
    }
    nomChoisi[j] = '\0';
    
    return 1; 
}

int chargerQCM(const char *nomQCM, QCM *qcm) {
    char nomFichier[300];
    creerNomFichier(nomQCM, nomFichier);
    
    FILE *f = fopen(nomFichier, "r");
    if (!f) return 0;

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
            printf("\n(Plusieurs reponses possibles. Entrez les numeros, puis 0 pour valider)\n");
            if (!qcm.modeSequentiel) printf("(Entrez -1 pour passer la question)\n");
            
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

            if (passerQuestion) continue; 

        } else {
            do {
                if (!qcm.modeSequentiel) printf("\nVotre choix (ou -1 pour passer) : ");
                else printf("\nVotre choix : ");

                if (scanf("%d", &saisie) != 1) {
                    while (getchar() != '\n');
                    printf("Saisie invalide.\n");
                    continue;
                }

                if (saisie == -1 && !qcm.modeSequentiel) break;
                if (saisie > 0 && saisie <= qcm.questions[i].nbChoix) {
                    reponsesEtudiant[saisie - 1] = 1;
                    break;
                } else {
                    printf("Numero invalide.\n");
                }
            } while (1);

            if (saisie == -1) continue;
        }

        int fautes = 0;
        for (int j = 0; j < qcm.questions[i].nbChoix; j++) {
            if (reponsesEtudiant[j] != qcm.questions[i].choix[j].estCorrect) fautes++;
        }

        if (fautes == 0) {
            totalPointsObtenus += pointsParQuestion;
        } else if (qcm.autoriserNegatif) {
            float penalite = ((float)fautes / qcm.questions[i].nbChoix) * pointsParQuestion;
            totalPointsObtenus -= penalite;
        }
    }

    if (totalPointsObtenus < 0) totalPointsObtenus = 0;

    printf("\n=========================================\n");
    printf("   FIN DU QCM. VOTRE NOTE : %.2f / 20.00\n", totalPointsObtenus);
    printf("=========================================\n");
}

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
                if (choisirQCM(nomQCM)) {
                    if (chargerQCM(nomQCM, &qcmEnCours)) {
                        passerQCM(qcmEnCours);
                    }
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
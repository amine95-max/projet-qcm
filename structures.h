#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MOT_DE_PASSE_DEFAUT "cytech2026"
#define MAX_TEXTE 256
#define MAX_CHOIX 10
#define MAX_QUESTIONS 50

typedef struct {
    char texte[MAX_TEXTE];
    int estCorrect; 
} Choix;

typedef struct {
    char texteQuestion[MAX_TEXTE];
    int nbChoix;
    Choix choix[MAX_CHOIX];
} Question;

typedef struct {
    char nom[MAX_TEXTE];
    int autoriserNegatif;     
    int reponsesMultiples;   
    int modeSequentiel;      
    int nbQuestions;
    Question questions[MAX_QUESTIONS];
} QCM;

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef STRUCTURES_H
#define STRUCTURES_H
#define TEACHER_PASSWORD "cytech2025"
#define MAX_TEXT 256
#define MAX_CHOICES 10
#define MAX_QUESTIONS 50

typedef struct {
    char text[MAX_TEXT];
    int isCorrect; // 1 = vrai, 0 = faux
} Choice;

typedef struct {
    char questionText[MAX_TEXT];
    int nbChoices;
    Choice choices[MAX_CHOICES];
} Question;

typedef struct {
    char name[MAX_TEXT];
    int allowNegative;     // 1 = oui, 0 = non
    int multipleAnswers;   // 1 = oui, 0 = non
    int sequentialMode;    // 1 = oui, 0 = non
    int nbQuestions;
    Question questions[MAX_QUESTIONS];
} QCM;

#endif
int teacherLogin() {
    char input[50];
    printf("Enter teacher password: ");
    scanf("%s", input);

    if (strcmp(input, TEACHER_PASSWORD) == 0) {
        printf("Access granted.\n");
        return 1;
    }
    printf("Access denied.\n");
    return 0;
}

// -------------------------------
// Fonction : créer un QCM
// -------------------------------
void createQCM(QCM *qcm) {
    printf("\n=== CREATE NEW QCM ===\n");

    printf("QCM name (no spaces): ");
    scanf("%s", qcm->name);

    printf("Allow negative points? (1=yes, 0=no): ");
    scanf("%d", &qcm->allowNegative);

    printf("Allow multiple correct answers? (1=yes, 0=no): ");
    scanf("%d", &qcm->multipleAnswers);

    printf("Sequential mode (must answer before next)? (1=yes, 0=no): ");
    scanf("%d", &qcm->sequentialMode);

    printf("Number of questions: ");
    scanf("%d", &qcm->nbQuestions);

    for (int i = 0; i < qcm->nbQuestions; i++) {
        printf("\n--- Question %d ---\n", i + 1);

        printf("Question text: ");
        getchar(); // clear buffer
        fgets(qcm->questions[i].questionText, MAX_TEXT, stdin);
        qcm->questions[i].questionText[strcspn(qcm->questions[i].questionText, "\n")] = 0;

        printf("Number of choices: ");
        scanf("%d", &qcm->questions[i].nbChoices);

        for (int j = 0; j < qcm->questions[i].nbChoices; j++) {
            printf("Choice %d text: ", j + 1);
            getchar();
            fgets(qcm->questions[i].choices[j].text, MAX_TEXT, stdin);
            qcm->questions[i].choices[j].text[strcspn(qcm->questions[i].choices[j].text, "\n")] = 0;

            printf("Is this choice correct? (1=yes, 0=no): ");
            scanf("%d", &qcm->questions[i].choices[j].isCorrect);
        }
    }

    printf("\nQCM created successfully.\n");
}

// -------------------------------
// Fonction : sauvegarder un QCM
// -------------------------------
void saveQCM(QCM qcm) {
    char filename[300];
    sprintf(filename, "%s.qcm", qcm.name);

    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error: cannot save file.\n");
        return;
    }

    fprintf(f, "%s\n", qcm.name);
    fprintf(f, "%d %d %d\n", qcm.allowNegative, qcm.multipleAnswers, qcm.sequentialMode);
    fprintf(f, "%d\n", qcm.nbQuestions);

    for (int i = 0; i < qcm.nbQuestions; i++) {
        fprintf(f, "%s\n", qcm.questions[i].questionText);
        fprintf(f, "%d\n", qcm.questions[i].nbChoices);

        for (int j = 0; j < qcm.questions[i].nbChoices; j++) {
            fprintf(f, "%s\n", qcm.questions[i].choices[j].text);
            fprintf(f, "%d\n", qcm.questions[i].choices[j].isCorrect);
        }
    }

    fclose(f);
    printf("QCM saved in file '%s'.\n", filename);
}

// -------------------------------
// Fonction principale du mode enseignant
// -------------------------------
void teacherMode() {
    if (!teacherLogin())
        return;

    QCM qcm;
    createQCM(&qcm);
    saveQCM(qcm);
}

int main() {
    int choice;

    printf("1. Teacher mode\n2. Student mode\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1)
        teacherMode();

    return 0;
}

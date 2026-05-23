# Gestionnaire de QCM - Projet CY Tech

## 📝 Description du projet
Ce projet est une application en langage C permettant la création, la gestion et l'évaluation automatisée de Questionnaires à Choix Multiples (QCM). 
Il a été développé dans le cadre du projet d'informatique de 1ère année de cycle préparatoire à CY Tech.

Le programme propose deux espaces distincts afin de répondre aux besoins des professeurs et des élèves.

## 🚀 Fonctionnalités implémentées

### Mode Enseignant (Sécurisé par mot de passe)
* **Authentification :** Accès protégé par un mot de passe (par défaut : `cytech2026`) modifiable depuis le menu.
* **Création de QCM :**  Définition du nom et du nombre de questions.
* **Paramétrage avancé du QCM :**
    * Activation ou désactivation des points négatifs.
    * Possibilité d'autoriser plusieurs bonnes réponses pour une question.
    * Mode séquentiel (imposer de répondre ou autoriser à passer la question).
* **Sauvegarde :** Enregistrement du QCM dans un fichier dédié (format `.qcm`) pour une réutilisation future[cite: 51, 57].

### Mode Étudiant
* **Menu interactif :** Détection et affichage dynamique de la liste des QCM disponibles dans le répertoire.
* **Passage de l'examen :** Prise en compte des règles spécifiques du QCM (réponses multiples, sauts de questions possibles si le mode séquentiel est désactivé).
* **Évaluation :** Calcul automatique du score tenant compte des erreurs et des points négatifs, avec restitution d'une note finale sur 20.

## 📁 Architecture des fichiers
Afin de respecter les bonnes pratiques de développement, le code source a été modularisé:
* `main.c` : Point d'entrée du programme, gère le menu principal de navigation.
* `enseignant.c` : Contient l'ensemble de la logique de création et de sauvegarde des examens.
* `etudiant.c` : Gère le parcours des fichiers, le passage des QCM et l'algorithme de notation.
* `structures.h` : Fichier d'en-tête regroupant les définitions des structures de données (`Choix`, `Question`, `QCM`) partagées entre les différents modules.
* `Makefile` : Règle les directives de compilation du projet.

## 🛠️ Compilation et Exécution

Ce projet intègre un `Makefile` pour automatiser la chaîne de compilation. 
Dans un terminal ouvert à la racine du projet, utilisez les commandes suivantes :

**1. Compiler le code source :**
```bash
make
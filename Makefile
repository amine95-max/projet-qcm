# Nom du programme final
EXEC = projet_qcm

# Compilateur à utiliser
CC = gcc

# Options de compilation (affiche les avertissements pour t'aider à corriger ton code)
CFLAGS = -Wall -Wextra

# Liste de tes fichiers sources
SRC = main.c enseignant.c etudiant.c

# Liste des fichiers objets (.o) générés automatiquement
OBJ = $(SRC:.c=.o)

# Règle par défaut (ce qui se lance quand tu tapes 'make')
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation de chaque fichier .c en fichier .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer le dossier (supprime l'exécutable et les .o)
clean:
	rm -f $(OBJ) $(EXEC)
# Makefile

# Définition des fichiers sources
SRCS = testidentite.c identite.c

# Définition des fichiers objets
OBJS = $(SRCS:.c=.o)

# Définition du compilateur
CC = gcc

# Définition des options de compilation
CFLAGS = -Wall -Wextra

# Nom de l'exécutable
TARGET = testidentite

# Règle de construction de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle de construction des fichiers objets (.o)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)
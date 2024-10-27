### Variables internes 
#  $@ : Le nom de la cible
#  $< : Le nom de la premi`ere d´ependance
#  $^ : La liste des d´ependances
#  $? : La liste des d´ependances plus réecentes que la cible
#  $* : Le nom du fichier sans suffixe


# Specific part of the Makefile
EXEC= stack_ex1 #le nom des .exe à générer 

# Begin generic part of the Makefile

### Variables Personnalisées
CC= gcc #compilateur utilisée
CFLAGS = -std=c99 -Werror -Wextra -Wall -pedantic #options de compilation 
LDFLAGS = #options de l'édition de lien 
SRC= $(wildcard *.c) #la liste des fichiers sources du projet
OBJ= $(SRC:.c=.o) #la liste des fichiers objets

ECHO = @
ifeq ($(VERBOSE),1)
	ECHO=
endif

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS =
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS =
endif

all:  
ifeq ($(DEBUG),yes)
	@echo "Generating in debug mode"
else
	@echo "Generating in release mode"
endif 
	@$(MAKE) $(EXEC)

# génération d'un .o à partir de son .c
%.o: %.c
	$(ECHO)$(CC) -o $@ -c $< $(CFLAGS)

# génération d'un .exe à partir des .o
$(EXEC): $(OBJ)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

### Cibles prédéfinis 
.PHONY: clean mrproper


clean: # supprime les fichiers intermédiaires 
	$(ECHO)rm -rf *.o

mrproper: clean # supprime tout ce qui peut ˆetre r´eg´en´er´e et permet une reconstruction complète du projet
	$(ECHO)rm -rf $(EXEC)

# End generic part of the makefile

# List of target dependencies
main.o: stack.h
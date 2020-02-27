
NAME = FileMatricialEncryption
LIB = `pkg-config gtk+-3.0 --libs` #Remplace par l'ensemble des librairies de GTK à link pour compiler
PATH_SRC = ./sources/
HEADER = ./includes/
FLAG = -O3 -Wall -Wextra -Werror `pkg-config gtk+-3.0 --cflags` #Remplace par l'ensemble des flags nécessaire à GTK
SRC = $(PATH_SRC)main.c  $(PATH_SRC)file.c $(PATH_SRC)functions_array.c $(PATH_SRC)gtk.c $(PATH_SRC)codage.c $(PATH_SRC)windows_dialog.c
OBJ = main.o file.o functions.o gtk.o codage.o windows_dialog.o


all: $(NAME)

$(NAME):
	gcc-8 $(FLAG() -I $(HEADER) $(SRC) -o $(NAME) $(LIB)

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)

re: fclean all

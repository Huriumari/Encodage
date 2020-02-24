#ifndef PROJECT_H
# define PROJECT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gtk/gtk.h>
#define SIZE_MAX 255
#define SIZE_MAX_WRITE 509

typedef struct	s_matrice{
	int		*data;
}				t_matrice;

typedef struct s_agument{
	char *file;
	t_matrice matrice;
}				t_argument;

char	**parse_file(char *file);
int        *convert_matrice(char *file);
char    *convert_file(char **array, int *width);
int		cmp_col(int *matrice, int mask_i, int mask_j, int heigh);
int		check_matrice(int *matrice, int heigh, int width);
int		f_arlen(char **array);
char	**f_realloc(char **array, char *new);
int		f_count_visual_char(char *str);
int		f_is_binaire(char **array);
int		f_count_elem(char **array);
void    selectFile(t_argument *argument);
int     f_encode_call(char *file, int *matrice);
int     f_decode_call(char *file, int *matrice);
void	print_matrice(int *matrice);
char	*openDialogWindow(void);




#endif
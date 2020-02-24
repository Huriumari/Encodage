#include "project.h"

int		f_arlen(char **array){
	int		i;

	if (!array)
		return 0;
	for (i = 0; array[i]; i++);
	return i;
}

char	**f_realloc(char **array, char *new){
	char	**new_array;
	int		i;

	if (!new || *new == '\0')
		return array;
	if (!array){
		new_array = (char**)malloc(sizeof(char*)*2);
		if (!new_array){
			fprintf(stderr, "Error: failed to allocate memory\n");
			return (0);
		}
		new_array[1] = 0; 
		new_array[0] = new;
	}else{
		new_array = (char**)malloc(sizeof(char*)*(f_arlen(array) + 2)); //space for the new one and ending
		if (!new_array){
			fprintf(stderr, "Error: failed to allocate memory\n");
			return (0);
		}
		for (i = 0; array[i]; i++){
			new_array[i] = array[i];
		}
		new_array[i] = new;
		i++;
		new_array[i] = 0;
		free(array);
	}
	return new_array;

}

char  **parse_file(char *path){
	FILE	*file;
	char	**array;
	char	*line;
	size_t	size;

	array = NULL;
	line = NULL;
	size = 0;
	file = fopen(path,"r+b");
	while (getline(&line, &size, file), !feof(file)){
		array = f_realloc(array, line);
	}
	return array;
} 


int		f_count_visual_char(char *str){
	int		result;

	for (result = 0; *str ; str++)
		if (!isspace((int)(*str)))
			result++;
	return result;
}

int		f_is_binaire(char **array){
	int		i,j;

	for (i = 0 ; array[i] ; i++)
		for (j = 0 ; array[i][j] ; j++)
			if (!isspace((int)array[i][j]) && array[i][j] != '0' && array[i][j] != '1')
				return 0;
	return 1;
}

int		f_count_elem(char **array){
	int		size;

	size = 0;
	while (*array){
		size+=strlen(*array);
		(*array)++;
	}
	return size;
}

#include "project.h"



char    *convert_file(char **array, int *width){
    char    *result;
    int        i,j,k;
    int        size;
    int        mask;

    size = f_count_elem(array);
    result = (char*)malloc(size / 8 + 1);
    *width = size;
    mask = 128;
    k = 0;
    for (i = 0 ; array[i] ; i++){
        for (j = 0 ; array[i][j] ; j++){
            if (array[i][j] == '1')
                result[k] += mask;
            mask = mask >> 1;
            if (!mask){
                mask = 128;
                k++;
            }

        }
    }
    return result;
}

int		cmp_col(int *matrice, int mask_i, int mask_j, int heigh){
	int		i;

	for (i = 0 ; i < heigh ; i++){
		if (matrice[i] & mask_i != matrice[i] & mask_j)
			return 0;
	}
	return 1;
}

int		check_matrice(int *matrice, int heigh, int width){
	int		i,j;
	int		mask;

	if (heigh > width)
		return 0;
	for (i = 0 ; i < heigh ; i++){				//check if the square matrice is correct
		mask = 1 << (width - 1 - i);
		if (!(matrice[i] & mask) || matrice[i] & ~mask)
			return 0;
	}											//check if all colomne are different
	for (i = width - 1 ; i > 1 ; i--)
		for (j = i - 1 ; j > 0 ; j--)
			if (cmp_col(matrice, 1 << i, 1 << j, heigh))
				return 0;
	return 1;
}

void	print_matrice(int *matrice){
	int		mask;
	int		i;

	for (i = 0 ; i < 4 ; i++){
		mask = 1 << 7;
		while (mask){
			if (matrice[i] & mask)
				printf("1");
			else
				printf("0");
			mask = mask >> 1;
		}
		printf("\n");
	}
}
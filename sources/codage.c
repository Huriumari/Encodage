#include "project.h"


int        *convert_matrice(char *file){
    int *matrice;
    int i,j,mask;
    int fd;
    char buffer[255];
    char *ptr;

    fd = open(file, O_RDONLY);
    if (fd == -1){
        printf("matrice non trouve\n");
        return 0;
    }
    read(fd, buffer, 254);
    matrice = (int*)malloc(sizeof(int)*4);
    matrice[0] = matrice[1] = matrice[2] = matrice[3] = 0;
    ptr = buffer;
    while (*ptr != '[')
        ptr++;
    for (i = 0 ; i < 4 ; i++){
        mask = 1 << 7;
        while (*ptr != ' ' && *ptr != ']'){
            if (*ptr == '1')
                matrice[i] += mask;
            mask = mask >> 1;
            ptr++;
        }
        ptr++;
    }
    close(fd);
    return matrice;
}


void    f_encode(char *buffer_read, char *buffer_write, int *matrice, int nb){
    int     mask_matrice;
    int     mask_file_read;
    int     mask_file_write;
    int     pos_read;
    int     j;
    int     pos_write;
    int     count;

    j = 0;
    mask_file_write = 1 << 7;
    mask_file_read = 1 << 7;
    pos_write = 0;
    for (pos_read = 0 ; pos_read < nb ; pos_read++){
        for (mask_matrice = 1 << 7 ; mask_matrice ; mask_matrice = mask_matrice >> 1){
            count = 0;
            for (j = 0 ; j < 4 ; j++){
                if ((matrice[j] & mask_matrice) && (buffer_read[pos_read] & mask_file_read))
                    count++;
                mask_file_read  = mask_file_read >> 1;
                if (!mask_file_read){
                    pos_read++;
                    mask_file_read = 1 << 7;
                }
            }
            if (count & 1)
                buffer_write[pos_write] += mask_file_write;
            mask_file_write = mask_file_write >> 1;
            if (!mask_file_write){
                mask_file_write = 1 << 7;
                pos_write++;
            }
        }
    }
}

int     f_encode_call(char *file, int *matrice){
    char        buffer_read[SIZE_MAX];
    FILE        *fd_read;
    FILE        *fd_write;
    char        buffer_write[SIZE_MAX_WRITE];
    size_t      size;

    fd_read = fopen(file, "r+b");

    if (fd_read == NULL){
        fprintf(stderr, "Error: failed to open %s\n",file);
        return 0;
    }
    strcpy(buffer_read, file);
    strcat(buffer_read, "c");

    fd_write = fopen(buffer_read, O_WRONLY | O_CREAT);
    if (fd_write == -1){
        fprintf(stderr, "Error: failed to creat %s\n",buffer_read);
        return 0;
    }
    chmod(buffer_read, S_IXUSR | S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH); 
    buffer_write[SIZE_MAX*2] = '\0';
    while (size = read(fd_read, buffer_read, SIZE_MAX - 1)){
        f_encode(buffer_read, buffer_write, matrice, (int)size);
        write(fd_write, buffer_write, (int)size * 2);
    }
    fclose(fd_read);

    fclose(fd_write);
    return 0;
}

char    **f_create_array(){
    char    **array;
    int     i;
    int     j;

    array = (char**)malloc(sizeof(char*) * 4);
    for (i = 0; i < 4 ; i++){
        array[i] = (char*)malloc(sizeof(char)*6);
        for (j = 0 ; j < 6 ; j++){
            array[i][j] = 0;
        }
    }
    return array;
}


//mask_1 / mask_2
char    **f_find_da_wae_in_matrice(int *matrice){
    int     mask_i;
    int     mask_j;
    int     count;
    int     i,j,k;
    char    **array;
    int     pos;
    int     nb = 0;

    array = f_create_array();
    for (mask_i = 1 << 7 ; mask_i && nb != 5; mask_i = mask_i >> 1){ //test si y'a un seul bit a 1 dans la ligne
        count = 0;
        for (i = 0 ; i < 4 ; i++){
            if (matrice[i] & mask_i){
                pos = i;
                count++;
            }
        }
        if (count == 1){
            array[pos][0] = nb;
            array[pos][1] = mask_i;
            nb++;
        }
    }

    for (mask_i = 1 << 7 ; mask_i && nb != 5; mask_i = mask_i >> 1){ //on cherche un bit de difference entre 2 lignes
        for (mask_j = mask_i >> 1 ; mask_j ; mask_j = mask_j >> 1){
            count = 0;
            for (i = 0 ; i < 4 ; i++){
                if ( (!(matrice[i] & mask_i) && (matrice[i] & mask_j))
                    || ((matrice[i] & mask_i) && !(matrice[i] & mask_j)) ){
                    pos = i;
                    count++;
                }
            }
            if (count == 1 && !array[pos][1]){
                array[pos][0] = nb;
                array[pos][1] = (char)mask_j;
                array[pos][2] = (char)mask_i;
                nb++;
            }
        }
    }

    for (j = 0 ; j < 4 ; j++){ //on exprime les derniers bit en fonction des autres
        for (mask_i = 1 << 7 ; mask_i && nb != 5 ; mask_i = mask_i >> 1){
            count = 0;
            for (i = 0 ; i < 4 ; i++){
                if ((matrice[i] & mask_i) && !array[i][0]){
                    pos = i;
                    count++;
                }
            }
            if (count == 1){
                array[pos][0] = nb;
                array[pos][1] = mask_i;
                k = 1;
                for (i = 0 ; i < 4 ; i++){
                    if ((matrice[i] & mask_i) && array[i][0])
                        array[pos][++k] = i;
                }
                array[pos][5] = k;
                nb++;
            }
        }
    }

    if (nb != 5){
        printf("Failed to find a method to resolve, file can't be decoded\n");
        for (i = 0 ; i < 4 ; i++)
            free(array[i]);
        free(array);
        exit(1);
    }
    return array;
}


int f_decode(char *buffer_read, char *buffer_write, int size, char **array_de_wae){
    int     i,j;
    int     mask_write;
    int     pos_write;
    int     pos_read;
    char    ordre;
    char    etat[4] = {0};
    int     count;

    pos_write = 0;
    mask_write = 1 << 7;
    for (pos_read = 0 ; pos_read < size ; pos_read++){
        for (i = 0 ; i < 4 ; i++)
            etat[i] = 0;
        for (ordre = 1 ; ordre < 5 ; ordre++){

            if (!mask_write){
                pos_write++;
                mask_write = 1 << 7;
            }
            for (i = 0 ; i < 4 ; i++){
                if (array_de_wae[i][0] == ordre){
                    if (!array_de_wae[i][2] && !array_de_wae[i][5]){
                        if (buffer_read[pos_read] & array_de_wae[i][1]){
                            etat[i] = 1;
                            buffer_write += mask_write;
                            break;
                        }
                    }
                    if (array_de_wae[i][2]){
                        if ((!(buffer_read[pos_read] & array_de_wae[i][1]) && 
                            (buffer_read[pos_read] & array_de_wae[i][0])) ||
                            ((buffer_read[pos_read] & array_de_wae[i][1]) &&
                            !(buffer_read[pos_read] & array_de_wae[i][0]))){

                            etat[i] = 1;
                            buffer_write += mask_write;
                            break;
                        }
                    }
                    count = 0;
                    for (j = 2 ; j < array_de_wae[i][5] ; j++){
                        if (etat[array_de_wae[i][j]])
                            count++;
                    }
                    count = count % 2;
                    if (count && !(array_de_wae[i][1] & buffer_read[pos_read]) ||
                        !count && (array_de_wae[i][1] & buffer_read[pos_read])){
                        etat[i] = 1;
                        buffer_write += mask_write;
                        break;

                    }
                }
            }
            mask_write = mask_write >> 1;
        }
    }

    return pos_write;
}

int     f_decode_call(char *file, int *matrice){
    char        buffer_read[SIZE_MAX_WRITE];
    int         fd_read;
    int         fd_write;
    char        buffer_write[SIZE_MAX_WRITE];
    size_t      size;
    char        **array_de_wae;

    fd_read = open(file, O_RDONLY);
    if (fd_read == -1){
        fprintf(stderr, "Error: failed to open %s\n",file);
        return 0;
    }
    strcpy(buffer_read, file);
    strcat(buffer_read, "d");
    fd_write = open(buffer_read, O_WRONLY | O_CREAT);
    if (fd_write == -1){
        fprintf(stderr, "Error: failed to creat %s\n",buffer_read);
        return 0;
    }
    chmod(buffer_read, S_IXUSR | S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH); 
    array_de_wae = f_find_da_wae_in_matrice(matrice);
    while (size = read(fd_read, buffer_read, SIZE_MAX_WRITE - 1)){
        size = f_decode(buffer_read, buffer_write, (int)size, array_de_wae);
        write(fd_write, buffer_write, (int)size + 1);
    }
    close(fd_read);
    close(fd_write);
    return 0;
}
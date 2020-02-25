#include "project.h"

void    print_encode(char *encode, size_t size){
    size_t          i;
    unsigned char   mask;

    for (i = 0 ; i < size ; i++){
        for (mask = 1 << 7 ; mask ; mask = mask >> 1){
            //printf("%d\n", (int)mask);
            if (encode[i] & mask)
                printf("1");
            else
                printf("0");
        }
        printf("\n");
    }
}

int     *convert_matrice(char *file){
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
    mask_file_read = 1 << 7;
    pos_write = 0;
    for (pos_read = 0 ; pos_read < nb ; pos_read++){
        mask_file_write = 1 << 7;
        buffer_write[pos_write] = 0;
        for (mask_matrice = 1 << 7 ; mask_matrice ; mask_matrice = mask_matrice >> 1){
            count = 0;
            mask_file_read = 1 << 7;
            for (j = 0 ; j < 4 ; j++){
                if (matrice[j] & mask_matrice && buffer_read[pos_read] & (mask_file_read >> j)){
                    count++;
                }
            }
            if (count & 1)
                buffer_write[pos_write] += mask_file_write;
            mask_file_write = mask_file_write >> 1;
        }
        pos_write++;
        buffer_write[pos_write] = 0;
        mask_file_write = 1 << 7;
        for (mask_matrice = 1 << 7 ; mask_matrice ; mask_matrice = mask_matrice >> 1){
            count = 0;
            mask_file_read = 1 << 3;
            for (j = 0 ; j < 4 ; j++){
                if (matrice[j] & mask_matrice && buffer_read[pos_read] & (mask_file_read >> j)){
                    count++;
                }
            }
            if (count & 1)
                buffer_write[pos_write] += mask_file_write;
            mask_file_write = mask_file_write >> 1;
        }
        pos_write++;
    }
}

int     f_encode_call(char *file, int *matrice){
    char        buffer_read[SIZE_MAX];
    FILE        *fd_read;
    FILE        *fd_write;
    char        buffer_write[SIZE_MAX_WRITE];
    size_t      size;

    fd_read = fopen(file, "rb+");

    if (fd_read == NULL){
        fprintf(stderr, "Error: failed to open %s\n",file);
        return 0;
    }
    strcpy(buffer_read, file);
    strcat(buffer_read, "c");

    fd_write = fopen(buffer_read, "wb+");
    if (fd_write == NULL){
        fprintf(stderr, "Error: failed to creat %s\n",buffer_read);
        return 0;
    }
    fchmod(fileno(fd_read), S_IXUSR | S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH); 
    buffer_write[SIZE_MAX*2] = '\0';

    while (42){
        size = fread(buffer_read, sizeof(char), SIZE_MAX - 1, fd_read);
        if (size == 0)
            break;
        f_encode(buffer_read, buffer_write, matrice, (int)size);
        fwrite(buffer_write, sizeof(char), size * 2, fd_write);
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

void    print_da_wae(char **array_da_wae){
    int i,j;

    for (i = 0 ; i < 4 ; i++){
        for (j = 0 ; j < 6 ; j++){
            printf("%d", (int)array_da_wae[i][j]);
        }
        printf("\n");
    }
}

char    **f_find_da_wae_in_matrice(int *matrice){
    int     mask_i;
    int     count;
    int     i;
    char    **array;
    int     pos;
    int     nb = 0;

    array = f_create_array();
    for (mask_i = 1 << 7 ; mask_i && nb != 5; mask_i = mask_i >> 1){
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
    return array;
}


int f_decode(char *buffer_read, char *buffer_write, int size, char **array_de_wae){
    int     i;
    int     mask_write;
    int     pos_write;
    int     pos_read;

    pos_read = 0;
    for (pos_write = 0 ; pos_read < size ; pos_write++){
        buffer_write[pos_write] = 0;
        for (i = 0 ; i < 4 ; i++){
            if (buffer_read[pos_read] & array_de_wae[i][1])
                buffer_write[pos_write] += (1 << (7 - i));
        }
        pos_read++;
        for (i = 0 ; i < 4 ; i++){
            if (buffer_read[pos_read] & array_de_wae[i][1])
                buffer_write[pos_write] += (1 << (3 - i));
        }
        pos_read++;
    }
    return pos_write;
}

int     f_decode_call(char *file, int *matrice){
    char        buffer_read[SIZE_MAX_WRITE];
    FILE        *fd_read;
    FILE        *fd_write;
    char        buffer_write[SIZE_MAX_WRITE];
    size_t      size;
    char        **array_de_wae;
    int         pos;

    fd_read = fopen(file, "rb+");
    if (fd_read == NULL){
        fprintf(stderr, "Error: failed to open %s\n",file);
        return 0;
    }

    strcpy(buffer_read, file);
    strcat(buffer_read, "d");

    fd_write = fopen(buffer_read, "wb+");
    if (fd_write == NULL){
        fprintf(stderr, "Error: failed to creat %s\n",buffer_read);
        return 0;
    }
    fchmod(fileno(fd_read), S_IXUSR | S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    array_de_wae = f_find_da_wae_in_matrice(matrice);
    print_da_wae(array_de_wae);
    printf("\n");
    print_matrice(matrice);
    while (42){
        size = fread(buffer_read, sizeof(char), SIZE_MAX - 1, fd_read);
        if (size == 0)
            break;
        pos = f_decode(buffer_read, buffer_write, (int)size, array_de_wae);
        fwrite(buffer_write, sizeof(char), pos, fd_write);
    }
    fclose(fd_read);
    fclose(fd_write);
    return 0;
}
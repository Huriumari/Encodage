#include "project.h"

void    signal_encrypt(GtkWidget *keyBtn, path_t *path){
    int     *matrice;

    if (keyBtn)
        keyBtn++;
    if (path->pathMatrix != NULL && path->pathFile != NULL){\
        matrice = convert_matrice(path->pathMatrix);
        f_encode_call(path->pathFile, matrice);
    }
}

void    signal_decrypt(GtkWidget *keyBtn, path_t *path){
    int     *matrice;

    if (keyBtn)
        keyBtn++;
    if (path->pathMatrix != NULL && path->pathFile != NULL){\
        matrice = convert_matrice(path->pathMatrix);
        f_decode_call(path->pathFile, matrice);
    }
}
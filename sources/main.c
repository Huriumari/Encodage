#include "project.h"

int main(int argc,char **argv)
{
    char command[25];
    char *pathMatrix;
    char *pathFile;
    int *matrix;

    gtk_init(&argc, &argv);

    pathMatrix = malloc(sizeof(char) * 249);
    pathFile = malloc(sizeof(char) * 249);

    printf("/crypt pour chiffrer\n/decrypt pour déchiffrer\n/key pour changer la clé\n/select pour sélectionner l'objet sur lequel effectuer le traitement\n/exit pour quitter\n\nCOMMAND :\n");

    do{

        fgets(command, 25, stdin);
        
        if(command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0';

        if(strcmp(command, "/crypt") == 0 && pathMatrix != NULL && pathFile != NULL){
            //fonction crypt
        }
        
        else if(strcmp(command, "/decrypt") == 0 && pathMatrix != NULL && pathFile != NULL){
            //fonction decrypt
        }

        else if(strcmp(command, "/key") == 0){
            pathMatrix = openDialogWindow();
            if(pathMatrix != NULL){
                printf("%s\n", pathMatrix);
            }
        }

        else if(strcmp(command, "/select") == 0){
            pathFile = openDialogWindow();
            if(pathFile != NULL){
                printf("%s\n", pathFile);
            }
        }

    }while(strcmp(command, "/exit"));

    free(pathMatrix);
    free(pathFile);
    return 0;

}

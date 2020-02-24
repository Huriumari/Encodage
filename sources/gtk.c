#include "project.h"
/*
void    selectFile(t_argument *argument){
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    int width;
    int height;
    int *matrice;
    GtkWindow * window = argument->window;
    char ***array = argument->array;

    dialog = gtk_file_chooser_dialog_new ("Open File", window, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        *array = parse_file(filename);
        matrice = convert_matrice(*array, &width, &height);
        printf("%p\n", matrice);
    }

    gtk_widget_destroy (dialog);
}*/
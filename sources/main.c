#include "project.h"

int main(int argc,char **argv)
{
    GtkWidget *window;
    GtkWidget *hBox;
    GtkWidget *buttonVBox;
    GtkWidget *encryptBtn;
    GtkWidget *decryptBtn;
    GtkWidget *selectBtn;
    GtkWidget *keyBtn;

    path_t *path = malloc(sizeof(path_t));
    char command[25];
    path->pathMatrix = NULL;
    path->pathFile = NULL;
    int *matrix;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "LogicSimButInC");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    hBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), hBox);

    buttonVBox = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonVBox), GTK_BUTTONBOX_END);
    gtk_box_pack_end(GTK_BOX(hBox), buttonVBox, TRUE, TRUE, 10);

    encryptBtn = gtk_button_new_with_label("Encrypt");
    gtk_container_add(GTK_CONTAINER(buttonVBox), encryptBtn);

    decryptBtn = gtk_button_new_with_label("Decrypt");
    gtk_container_add(GTK_CONTAINER(buttonVBox), decryptBtn);
    
    selectBtn = gtk_button_new_with_label("Select File");
    gtk_container_add(GTK_CONTAINER(buttonVBox), selectBtn);

    keyBtn = gtk_button_new_with_label("Select key");
    gtk_container_add(GTK_CONTAINER(buttonVBox), keyBtn);

    g_signal_connect(G_OBJECT(encryptBtn), "clicked", G_CALLBACK(signal_encrypt), path);
    g_signal_connect(G_OBJECT(decryptBtn), "clicked", G_CALLBACK(signal_decrypt), path);
    g_signal_connect(G_OBJECT(selectBtn), "clicked", G_CALLBACK(openFileDialogWindow), path);
    g_signal_connect(G_OBJECT(keyBtn), "clicked", G_CALLBACK(openKeyDialogWindow), path);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;

}

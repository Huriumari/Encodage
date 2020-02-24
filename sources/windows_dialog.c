#include "project.h"

void openFileDialogWindow(path_t *path){

  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  gchar *filename; 

  dialog = gtk_file_chooser_dialog_new ("Open File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));

  if (res == GTK_RESPONSE_ACCEPT){

    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    path->pathFile = filename;
    printf("%s\n", path->pathFile);
    g_free(filename);

  }
  
  gtk_widget_destroy(dialog);

}

void openKeyDialogWindow(path_t *path){

  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  gchar *filename; 

  dialog = gtk_file_chooser_dialog_new ("Open File", NULL, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));

  if (res == GTK_RESPONSE_ACCEPT){

    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    path->pathMatrix = filename;
    printf("%s\n", path->pathMatrix);
    g_free(filename);

  }
  
  gtk_widget_destroy(dialog);

}
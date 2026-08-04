#include <gtk/gtk.h>

static void pressed_cb (GtkGestureClick *gesture,
         int              n_press,
         double           x,
         double           y,
         GFileInfo       *file_info){
    GFile *file = G_FILE (g_file_info_get_attribute_object (file_info,"standard::file"));
    printf("%s\n", g_file_get_path (file));
}

static void setup_listitem_cb (GtkListItemFactory *factory,GtkListItem *list_item){
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,20);
  GtkWidget *image;
  GtkWidget *label = gtk_label_new(NULL);
  image = gtk_image_new ();
  gtk_image_set_icon_size (GTK_IMAGE (image), GTK_ICON_SIZE_LARGE);
  gtk_box_append (GTK_BOX (box), image);
  gtk_box_append (GTK_BOX (box), label);
  gtk_list_item_set_child (list_item, box);
}

static void
bind_listitem_cb (GtkListItemFactory *factory, GtkListItem *list_item){
  GtkWidget *box = gtk_list_item_get_child (list_item);
  GFileInfo *file_info = gtk_list_item_get_item (list_item);
  GtkImage *image = GTK_IMAGE (gtk_widget_get_first_child(box));
  GtkLabel *label = GTK_LABEL (gtk_widget_get_last_child(box));
  gtk_image_set_from_gicon (GTK_IMAGE (image),g_file_info_get_icon(file_info));

  gtk_label_set_text (GTK_LABEL (label), g_file_info_get_name(file_info));
  GtkGesture *gesture = gtk_gesture_click_new ();
  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (gesture), GDK_BUTTON_SECONDARY);
  gtk_widget_add_controller (box, GTK_EVENT_CONTROLLER (gesture));
  g_signal_connect (gesture, "pressed", G_CALLBACK (pressed_cb), file_info);
}

static void grid_activate (GtkGridView *grid, int position, gpointer user_data) {
  GFileInfo *info = G_FILE_INFO (g_list_model_get_item (G_LIST_MODEL (gtk_grid_view_get_model (grid)), position));
 // launch_tfe_with_file (info);
 printf("hi\n");
}

static void app_activate (GApplication *app, gpointer *user_data) {
  GtkBuilder *builder = gtk_builder_new_from_file ("sktSender.ui");
  GtkWidget *win = GTK_WIDGET (gtk_builder_get_object (builder, "win"));
  GtkWidget *nb = GTK_WIDGET (gtk_builder_get_object (builder, "nb"));
  gtk_window_set_application (GTK_WINDOW (win), GTK_APPLICATION (app));

  GFile *file = g_file_new_for_path ("/home/imaxii");
  GtkDirectoryList *dl = gtk_directory_list_new ("standard::*", file);
  g_object_unref (file);
  GtkSingleSelection *model = gtk_single_selection_new (G_LIST_MODEL (dl));
  GtkListItemFactory *factory = gtk_signal_list_item_factory_new ();
  g_signal_connect (factory, "setup", G_CALLBACK (setup_listitem_cb), NULL);
  g_signal_connect (factory, "bind", G_CALLBACK (bind_listitem_cb), NULL);
  GtkGridView *grid = GTK_GRID_VIEW(gtk_builder_get_object (builder, "grid"));
  gtk_grid_view_set_factory (grid, factory);
  gtk_grid_view_set_model (grid, GTK_SELECTION_MODEL (model));
  g_object_ref (grid);
  g_signal_connect (GTK_GRID_VIEW (grid), "activate", G_CALLBACK (grid_activate), NULL);
  gtk_window_present (GTK_WINDOW (win));
}

int main (int argc, char **argv) {
  GtkApplication *app;
  int stat;
  app = gtk_application_new ("com.github.anachuri.sktSender", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
  stat = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return stat;
}

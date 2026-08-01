/* sktsender-window.c
 *
 * Copyright 2026 maxi anachuri
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "sktsender-window.h"

struct _SktsenderWindow
{
  GtkApplicationWindow parent_instance;
  /* Template widgets */
  GtkGridView *grid;
};

G_DEFINE_FINAL_TYPE (SktsenderWindow, sktsender_window, GTK_TYPE_APPLICATION_WINDOW)

char *
get_file_name (GtkListItem *item, GFileInfo *info){
  return G_IS_FILE_INFO (info) ? g_strdup (g_file_info_get_name (info)) : NULL;
}

GIcon *
get_icon (GtkListItem *item, GFileInfo *info){
  GIcon *icon;
  /* g_file_info_get_icon can return NULL */
  icon = G_IS_FILE_INFO (info) ? g_file_info_get_icon (info) : NULL;
  return icon ? g_object_ref (icon) : NULL;
}

static void
setup_listitem_cb (GtkListItemFactory *factory,
                   GtkListItem        *list_item){
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
  GtkWidget *image;
  GtkWidget *label = gtk_label_new(NULL);
  image = gtk_image_new ();
  gtk_image_set_icon_size (GTK_IMAGE (image), GTK_ICON_SIZE_LARGE);
  gtk_box_append (GTK_BOX (box), image);
  gtk_box_append (GTK_BOX (box), label);
  gtk_list_item_set_child (list_item, box);

}

static void pressed_cb (GtkGestureClick *gesture,
         int              n_press,
         double           x,
         double           y,
         GtkWidget       *area)
{
  printf ("hila pololola\n");
}

static void
bind_listitem_cb (GtkListItemFactory *factory, GtkListItem *list_item)
{
  GtkWidget *box;
  //GFile *file;
  box = gtk_list_item_get_child (list_item);
 // file = gtk_list_item_get_item (list_item);
 // gtk_image_set_from_gicon (GTK_IMAGE (image),g_app_info_get_icon (app_info));

  GtkGesture *gesture = gtk_gesture_click_new ();
  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (gesture), GDK_BUTTON_SECONDARY);
  gtk_widget_add_controller (box, GTK_EVENT_CONTROLLER (gesture));
  g_signal_connect (gesture, "pressed", G_CALLBACK (pressed_cb), box);
}


static void
grid_activate (GtkGridView *grid, int position, gpointer user_data) {
  GFileInfo *info = G_FILE_INFO (g_list_model_get_item (G_LIST_MODEL (gtk_grid_view_get_model (grid)), position));
 // launch_tfe_with_file (info);
 printf("hi\n");
}

static void
sktsender_window_class_init (SktsenderWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/github/anachuri/sktSender/sktsender-window.ui");
  gtk_widget_class_bind_template_child (widget_class, SktsenderWindow, grid);
  gtk_widget_class_bind_template_callback (widget_class, get_file_name);
  gtk_widget_class_bind_template_callback (widget_class, get_icon);
}

static void sktsender_window_init (SktsenderWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
  GFile *file = g_file_new_for_path ("/home/imaxii");
  GtkDirectoryList *dl = gtk_directory_list_new ("standard::name", file);
  g_object_unref (file);
  GtkSingleSelection *model = gtk_single_selection_new (G_LIST_MODEL (dl));
  const char *ui_string =
"<interface>"
  "<template class=\"GtkListItem\">"
    "<property name=\"child\">"
      "<object class=\"GtkLabel\">"
        "<binding name=\"label\">"
          "<closure type=\"gchararray\" function=\"get_file_name\">"
            "<lookup name=\"item\">GtkListItem</lookup>"
          "</closure>"
        "</binding>"
      "</object>"
    "</property>"
  "</template>"
"</interface>"
;
  //GBytes *gbytes = g_bytes_new_static (ui_string, strlen (ui_string));
  //GtkListItemFactory *factory = gtk_builder_list_item_factory_new_from_bytes (NULL, gbytes);
  GtkListItemFactory *factory = gtk_signal_list_item_factory_new ();
  g_signal_connect (factory, "setup", G_CALLBACK (setup_listitem_cb), NULL);
  g_signal_connect (factory, "bind", G_CALLBACK (bind_listitem_cb), NULL);


  gtk_grid_view_set_factory (self->grid, factory);
  gtk_grid_view_set_model (self->grid, GTK_SELECTION_MODEL (model));
  g_object_ref (self->grid);
  g_signal_connect (GTK_GRID_VIEW (self->grid), "activate", G_CALLBACK (grid_activate), NULL);
}




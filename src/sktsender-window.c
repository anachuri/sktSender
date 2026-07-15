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


static void
sktsender_window_class_init (SktsenderWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/github/anachuri/sktSender/sktsender-window.ui");
  // gtk_widget_class_bind_template_child (widget_class, SktsenderWindow, label);

}

static void
sktsender_window_init (SktsenderWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  // gtk_widget_class_bind_template_child (widget_class, SktsenderWindow, self->grid);

  GFile *file = g_file_new_for_path (".");
  GtkDirectoryList *dl = gtk_directory_list_new ("standard::name", file);
  g_object_unref (file);
  GtkNoSelection *model = gtk_no_selection_new (G_LIST_MODEL (dl));
  gtk_grid_view_set_model (self->grid, GTK_SELECTION_MODEL (model));
}



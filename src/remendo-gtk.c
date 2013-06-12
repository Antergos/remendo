/*
 * remendo-gtk.c
 *
 * Copyright (C) 2013 - Alex Filgueira
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>


void window()
{
	GtkWidget * ventana;
	GtkWidget * etiqueta;

	gtk_init (NULL, NULL);
	ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	etiqueta = gtk_label_new("prueba");
	gtk_container_add ((GtkContainer *) ventana, etiqueta);
	
	gtk_window_set_title((GtkWindow *) ventana, "Remendo");

	
	gtk_widget_show(etiqueta);
	gtk_widget_show(ventana);

	gtk_main();
}
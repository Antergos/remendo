# -*- Mode: Python; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- #!/usr/bin/python
#
# main.py
# Copyright (C) 2013 Alexandre Filgueira <alexfilgueira@antergos.com>
# 
# remendo-gtk is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# remendo-gtk is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

from gi.repository import Gtk, GdkPixbuf, Gdk, Pango
import os, sys
import xml.etree.ElementTree as etree


#Comment the first line and uncomment the second before installing
#or making the tarball (alternatively, use project variables)
UI_FILE = "remendo_gtk.ui"
#UI_FILE = "/usr/local/share/remendo_gtk/ui/remendo_gtk.ui"


class GUI:
	
	def __init__(self):

		self.builder = Gtk.Builder()
		self.builder.add_from_file(UI_FILE)
		self.builder.connect_signals(self)

		self.window = self.builder.get_object('remendo')
		self.event_treeview = self.builder.get_object('event_treeview')
		self.set_event_list()

		self.window.show_all()

	def on_remendo_destroy(window, self):
		Gtk.main_quit()

	def set_event_list(self):
		event_liststore = Gtk.ListStore(str, str)

		render_name = Gtk.CellRendererText()
		render_name.props.weight_set=True
		render_name.props.weight=Pango.Weight.BOLD
		event_name_col = Gtk.TreeViewColumn("Event", render_name, text=0)
		render_info = Gtk.CellRendererText()
		event_col = Gtk.TreeViewColumn("Info", render_info, text=1)
		self.event_treeview.set_model(event_liststore)
		self.event_treeview.append_column(event_name_col)
		self.event_treeview.append_column(event_col)
		self.populate_events(event_liststore)

	def populate_events(self, event_liststore):
		tree = etree.parse('../../remendo/database.db')
		root = tree.getroot()

		for event in root.findall('event'):
			name = event.find('name').text
			description = event.find('description').text
			url_script = event.find('url_script').text
			event_liststore.append([name, "Description: \n" + description + \
									"\n" + "Script: " + url_script])
		

def main():
	app = GUI()
	Gtk.main()
		
if __name__ == "__main__":
	sys.exit(main())


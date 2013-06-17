# -*- Mode: Python; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- #!/usr/bin/python
#
# remendo_gtk.py
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

from gi.repository import Gtk, Pango, GObject, Gdk
import os,sys
import xml.etree.ElementTree as etree
from urllib.request import urlopen
from subprocess import Popen
from threading import Thread


#UI_FILE = "remendo_gtk.ui"
UI_FILE = "/usr/share/remendo_gtk/ui/remendo_gtk.ui"
DATABASE = '/usr/share/remendo/database.db'

class Script(Thread):
	def __init__(self):
		self.url_script = ''
		self.local_script = ''
		self.running = True
		Thread.__init__(self)
	def run(self):
		script = urlopen(self.url_script)
		localscript = open(self.local_script, 'wb')
		localscript.write(script.read())
		localscript.close()
		
		command = ['bash', self.local_script]
		p = Popen(command)
		p.communicate()
		self.running = False

class GUI:
	
	def __init__(self):

		self.builder = Gtk.Builder()
		self.builder.add_from_file(UI_FILE)
		self.builder.connect_signals(self)

		self.window = self.builder.get_object('remendo')
		self.event_treeview = self.builder.get_object('event_treeview')
		self.event_info = self.builder.get_object('event_info')
		self.spinner = self.builder.get_object('spinner')
		self.spinner.hide()

		self.selected_event = ''
		self.url_script = ''
		self.local_script = ''

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
		tree = etree.parse(DATABASE)
		root = tree.getroot()

		for event in root.findall('event'):
			if event.find('state').text not in 'Fixed, Skipped':
				name = event.find('name').text
				description = event.find('description').text
				url_script = event.find('url_script').text
				event_liststore.append([name, "Description: \n" + description + \
										"\n" + "Script: " + url_script])

	def on_event_treeview_cursor_changed(self, treeview):
		selected = treeview.get_selection()
		if selected:
			(ls, iter) = selected.get_selected()
			if iter:
				self.selected_event = ls.get_value(iter, 0)
				self.url_from_selected()

	def on_fixme_button_clicked(self, button):
		self.spinner.start()
		self.s = Script()
		self.s.local_script = '/tmp/%s.sh' % self.selected_event.replace(" ", "_")
		self.s.url_script = self.url_script
		self.s.start()
		self.run_script()
		GObject.timeout_add(200, self.run_script)
		
		
	def on_manually_button_clicked(self, button):
		tree = etree.parse(DATABASE)
		xpath = './/event[name="%s"]/state' % self.selected_event
		tree.find(xpath).text = 'Skipped'
		tree.write(DATABASE)
		self.set_event_list()

	def url_from_selected(self):
		tree = etree.parse(DATABASE)
		xpath = './/event[name="%s"]/url_script' % self.selected_event
		self.url_script = tree.find(xpath).text

	def run_script(self):
		if self.s.running:
			self.event_info.set_label("Fixing now...")
		if self.s.running is False:
			self.event_solved()
			self.spinner.stop()
			self.event_info.set_label("My work is done. Have a nice day!")
			return False
		return True
			
	def event_solved(self):
		tree = etree.parse(DATABASE)
		xpath = './/event[name="%s"]/state' % self.selected_event
		tree.find(xpath).text = 'Fixed'
		tree.write(DATABASE)
		self.set_event_list()


def main():
	GObject.threads_init()
	app = GUI()
	Gdk.threads_enter()
	Gtk.main()
	Gdk.threads_leave()
		
if __name__ == "__main__":
	sys.exit(main())


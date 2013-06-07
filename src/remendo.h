/*
 * remendo.h
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

const char *antergos_xml = "http://install.antergos.com/events.xml";
const char *archlinux_xml = "https://www.archlinux.org/feeds/news/";

const char *remendo_events_file = "/tmp/remendo-events.xml";
const char *archlinux_events_file = "/tmp/archlinux-events.xml";

static void displayInotifyEvent(struct inotify_event *i);

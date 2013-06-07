/***************************************************************************
 *            xml_handler.h
 *
 *  vie junio 07 19:05:09 2013
 *  Copyright  2013  Alex Filgueira
 *  <user@host>
 ****************************************************************************/
/*
 * xml_handler.h
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

#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

void get_events(xmlDocPtr doc, xmlNodePtr cur, char *subchild);
int parse_xml(char *xml_file, char *child, char *subchild);


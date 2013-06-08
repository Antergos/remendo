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

#ifndef XML_HANDLER_H
#define XML_HANDLER_H

#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#define ENCODING "ISO-8859-1"

char *db_uri = "database.db";

char timeToString(time_t time);
char getCurrentTimeString();
int checkDatabase();
int createDatabase();
void get_node(xmlDocPtr doc, xmlNodePtr cur, char *subchild);
xmlChar *get_attribute(xmlNodePtr cur, char *search_for);
xmlChar *getCreation();
int parse_xml(char *xml_file, char *child, char *subchild);
void checkEvents();

#endif

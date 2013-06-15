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
int pending_events = 0;

char timeToString(time_t time);
char getCurrentTimeString();
int checkDatabase();
int createDatabase();
xmlChar *get_node(xmlDocPtr doc, xmlNodePtr cur, char *subchild);
xmlChar *get_attribute(xmlNodePtr cur, char *search_for);
xmlChar *getCreation(char *xml_file, const xmlChar *node);
int parse_xml(char *xml_file, char *child, char *db_creation);
int checkNewEvents(char *xml_file);
int saveNewEvent(char *keyword, char *id, char *value, int type);
int parseName(xmlDocPtr doc, xmlNodePtr cur, char *keyword, char *value);
unsigned long dateCharToInt(char *value);

#endif

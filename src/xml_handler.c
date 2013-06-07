/*
 * xml_handler.c
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

#include "xml_handler.h"

void get_events(xmlDocPtr doc, xmlNodePtr cur){
    xmlChar *id=NULL, *name=NULL, *description=NULL, *url=NULL;
    cur=cur->xmlChildrenNode;
    while(cur != NULL){
        if(!xmlStrcmp(cur->name, (const xmlChar*)"name")){
            // printf("%s\n", xmlNodeListGetString(doc,cur->xmlChildrenNode, 1));
			name = xmlNodeListGetString(doc,cur->xmlChildrenNode, 1);
        }
        cur=cur->next;
    }
	printf("%s\n", name);
}

int parse_xml(char xml_file){
    xmlDocPtr doc; /* the resulting document tree */
    xmlNodePtr cur;

    doc = xmlParseFile(xml_file);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", xml_file);
    return;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL){
        fprintf(stderr, "Empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    cur=cur->xmlChildrenNode;
    int count=0;
    while(cur != NULL){
        if(!xmlStrcmp(cur->name, (const xmlChar *)"event")){
            get_events(doc,cur);
            ++count;
        }
        cur=cur->next;
    }
    xmlFreeDoc(doc);
}
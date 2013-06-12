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

#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xml_handler.h"
#include "remendo-gtk.h"

/*Creates database.db if doesn't exist.
 This is the database where remendo stores which events
 have been fixed, or not fixed*/
int checkDatabase(){
	// Create database.db if doesn't exists
	if(access(db_uri, F_OK) == -1) {
		printf("File database.db not found. Creating it now...\n");
		if(createDatabase() == 0){
			printf("Database created.\n");
		}
		else{
			printf("Something went wrong while creating database.db\n");
			return 1;
		}
	}
	return 0;
}

int createDatabase(){
	time_t now;
	struct tm * timeinfo;
	int rc;
	xmlTextWriterPtr writer;

	writer = xmlNewTextWriterFilename(db_uri, 0);
	if(writer == NULL){
		printf("Error creating the xml writer\n");
		return 1;
	}

	rc = xmlTextWriterStartDocument(writer, NULL, ENCODING, NULL);
	if(rc < 0){
		printf("Error starting the document\n");
		return 1;
	}

	rc = xmlTextWriterStartElement(writer, BAD_CAST "remendo_db");
	if(rc < 0){
		printf("Error creating the root element in the XML file\n");
		return 1;
	}

	now = time(0);
	timeinfo = localtime (&now);
	xmlChar string_date [80];
	strftime(string_date,80,"%s",timeinfo);
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "creation", string_date);
	if(rc < 0){
		printf("Error creating attribute of root element\n");
		return 1;
	}

	rc = xmlTextWriterEndElement(writer);
	if(rc < 0){
		printf("Error ending the XML file\n");
		return 1;
	}

	xmlFreeTextWriter(writer);
	xmlCleanupParser();
	return 0;
}
	

xmlChar *get_node(xmlDocPtr doc, xmlNodePtr cur, char *subchild){
    xmlChar *key=NULL;
    cur = cur->xmlChildrenNode;
    while(cur != NULL){
        if((!xmlStrcmp(cur->name, (const xmlChar *)subchild))){
			key = xmlNodeListGetString(doc,cur->xmlChildrenNode, 1);
		}
        cur = cur->next;
    }
	return(key);
}

xmlChar *get_attribute(xmlNodePtr cur, char *search_for){
	xmlAttr* attribute = cur->properties;
	xmlChar* value;
	
	while(attribute && attribute->name && attribute->children){
		if(strcmp(attribute->name, search_for) == 0){
			value = xmlNodeListGetString(cur->doc, attribute->children, 1);
		}
		attribute = attribute->next;
	}
	return(value);
}

/*Get creation attribute from databases. 
This value is used to know which events can be fixed.
The value is a date*/
xmlChar *getCreation(char *xml_file, const xmlChar *node){
	printf("Searching for creation date in %s...\n",node);

	xmlDocPtr doc;
    xmlNodePtr cur;
	xmlChar *attribute;

    doc = xmlParseFile(xml_file);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", xml_file);
		return 1;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL){
        fprintf(stderr, "Empty document\n");
        xmlFreeDoc(doc);
        return 1;
    }
	
    while(cur != NULL){
        if((!xmlStrcmp(cur->name, (const xmlChar *)node))){
			char *cre = "creation";
			attribute = get_attribute(cur, cre);
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
	return(attribute);
}

int parse_xml(char *xml_file, char *child, char *db_creation){
    xmlDocPtr doc;
    xmlNodePtr cur;
	char *eventCreation;

	printf("Searching for events...\n");

    doc = xmlParseFile(xml_file);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", xml_file);
		return 1;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL){
        fprintf(stderr, "Empty document\n");
        xmlFreeDoc(doc);
        return 1;
    }

	cur = cur->xmlChildrenNode;
    while(cur != NULL){
        if((!xmlStrcmp(cur->name, (const xmlChar *)child))){
			eventCreation = get_attribute(cur, "creation");

			if(dateCharToInt(db_creation) <= dateCharToInt(eventCreation)){
				printf("New event: '%s' to %s...\n", get_node(doc,cur,"name"), db_uri);
				saveNewEvent("event", NULL,0);
				saveNewEvent("name", get_node(doc,cur,"name"),1);
				saveNewEvent("description", get_node(doc,cur,"description"),1);
				saveNewEvent("url_script", get_node(doc,cur,"url_script"),1);
			}
        }
        cur = cur->next;
    }
	
    xmlFreeDoc(doc);
	return 0;
}

void checkNewEvents(char *xml_file){
	xmlChar *db_creation;
	db_creation = getCreation(db_uri, (const xmlChar *)"remendo_db");
	parse_xml(xml_file, "event", db_creation);
}

void saveNewEvent(char *keyword, char *value, int type){
    xmlDocPtr doc;
    xmlNodePtr cur;
	

    doc = xmlParseFile(db_uri);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", db_uri);
		return 1;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL){
        fprintf(stderr, "Empty document\n");
        xmlFreeDoc(doc);
        return 1;
    }

	if(type == 1) cur = cur->xmlChildrenNode;
	
    while(cur != NULL){
		if(type == 0){
    		if((!xmlStrcmp(cur->name, (const xmlChar *)"remendo_db"))){
				xmlNewTextChild(cur, NULL, keyword, value);
			}
        }else{
			if((!xmlStrcmp(cur->name, (const xmlChar *)"event"))){
				xmlNewTextChild(cur, NULL, keyword, value);
			}
		}
        cur = cur->next;
    }
	xmlSaveFormatFile(db_uri, doc, 1);
    xmlFreeDoc(doc);
}

unsigned long dateCharToInt(char *value){
	unsigned long x = strtoul(value, NULL, 10);
	return x;
}
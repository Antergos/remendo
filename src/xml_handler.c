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

#include "xml_handler.h"


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
	

void get_node(xmlDocPtr doc, xmlNodePtr cur, char *subchild){
    xmlChar *key=NULL;
    cur = cur->xmlChildrenNode;
    while(cur != NULL){
        if((!xmlStrcmp(cur->name, (const xmlChar *)subchild))){
			key = xmlNodeListGetString(doc,cur->xmlChildrenNode, 1);
			printf("%s\n", key);
			xmlFree(key);
		}
        cur = cur->next;
    }
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

xmlChar *getCreation(const xmlChar *node){
	printf("Searching for creation date in %s...\n",db_uri);

	xmlDocPtr doc;
    xmlNodePtr cur;
	xmlChar *attribute;

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

int parse_xml(char *xml_file, char *child, char *subchild){
    xmlDocPtr doc;
    xmlNodePtr cur;

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

	if(strcmp(subchild, "creation") != 0){
		cur = cur->xmlChildrenNode;
	}
	
    while(cur != NULL){
        if((!xmlStrcmp(cur->name, (const xmlChar *)child))){	
			get_node(doc,cur,subchild);
        }
        cur = cur->next;
    }
    xmlFreeDoc(doc);
	return 0;
}

void checkEvents(){
	xmlChar *db_creation;
	db_creation = getCreation((const xmlChar *)"remendo_db");
}
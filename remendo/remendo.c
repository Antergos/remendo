/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * remendo.c
 * Copyright (C) 2013 Alexandre Filgueira <alexfilgueira@antergos.com>
 * 
 * remendo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * remendo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/inotify.h>
#include <limits.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

#include "remendo.h"
#include "dl_events.h"

#ifndef XML_HANDLER_H
#define XML_HANDLER_H
#include "xml_handler.h"
#endif

void print_usage(char *argv[]){
	printf( "usage: %s <interface>\n", argv[0] );
	printf("Interfaces:\n");
	printf("\tremendo --gtk\n");
	printf("\tremendo --qt\n");
}

static void displayInotifyEvent(struct inotify_event *i, char *argv[]){
	
    if (i->mask & IN_OPEN){
        printf("Pacman executed\n");

		// Download first our XML file
        get_event_list(antergos_xml, remendo_events_file);
		// Download Arch Linux XML file
		// get_event_list(archlinux_xml, archlinux_events_file);

		// Check for unhandled events
		if(checkNewEvents(remendo_events_file) == 1){
			if(strcmp(argv[1], "--gtk") == 0){
				system("remendo_gtk.py");
			}else{
				printf("QT not implemented yet\n");
			}
		}
    }        
    printf("\n");
}

int main(int argc, char *argv[]){
    int inotifyFd, wd, j;
    char buf[BUF_LEN];
    ssize_t numRead;
    char *p;
    struct inotify_event *event; 

	if(argc != 2){
		print_usage(argv);
    }else{
		if((strcmp(argv[1], "--gtk") != 0) && (strcmp(argv[1], "--qt") != 0)){
			print_usage(argv);
		}else{

			checkDatabase();

			inotifyFd = inotify_init();
			if ( inotifyFd < 0 ) {
				perror( "inotify_init" );
			}

			/* Watch pacman binary */
			wd = inotify_add_watch( inotifyFd, "/usr/bin/pacman", IN_OPEN );
			if (wd == -1)
				perror("inotify_add_watch");

			printf("Watching '/usr/bin/pacman' using wd %d\n", wd);


			for (;;) {
				numRead = read(inotifyFd, buf, BUF_LEN);

				if ( numRead < 0 ) {
				    perror( "read" );
				}

				// Process events in buffer returned by read()
				for (p = buf; p < buf + numRead; ) {
				    event = (struct inotify_event *) p;
				    displayInotifyEvent(event, argv);

				    p += sizeof(struct inotify_event) + event->len;
				}
			}
		}
	}
}

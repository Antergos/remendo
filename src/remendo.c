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

#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>

#include "remendo.h"
#include "xml_handler.h"


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream){
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int get_event_list(){
    CURL *curl;
    FILE *f;
    CURLcode res;
    char *url = "http://install.antergos.com/events.xml";
    curl = curl_easy_init();
    if (curl){
        printf("Downloading XML file...\n");

        f = fopen(remendo_events_file,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
        res = curl_easy_perform(curl);

        // cleanup
        curl_easy_cleanup(curl);

        fclose(f);

        printf("Download completed!\n");

        return 0;
    }
    else{
        printf("Failed downloading events.xml\n");
        return 1;
    }
}

static void             /* Display information from inotify_event structure */
displayInotifyEvent(struct inotify_event *i)
{
    if (i->mask & IN_OPEN){
        printf("Pacman executed\n");
        get_event_list();
        parse_xml(remendo_events_file);
    }        
    printf("\n");
}

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int
main()
{
    int inotifyFd, wd, j;
    char buf[BUF_LEN];
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    inotifyFd = inotify_init();                 /* Create inotify instance */
    if ( inotifyFd < 0 ) {
        perror( "inotify_init" );
    }

    /* Watch pacman binary */
    wd = inotify_add_watch( inotifyFd, "/usr/bin/pacman", IN_OPEN );
    if (wd == -1)
        perror("inotify_add_watch");

    printf("Watching '/usr/bin/pacman' using wd %d\n", wd);


    for (;;) {                                  /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);

        if ( numRead < 0 ) {
            perror( "read" );
        }

        /* Process events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }
}

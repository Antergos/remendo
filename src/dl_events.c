/*
 * dl_events.c
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

#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>

#include "dl_events.h"


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream){
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int get_event_list(const char *url, const char *file){
    CURL *curl;
    FILE *f;
    CURLcode res;
    curl = curl_easy_init();
    if (curl){
        printf("Downloading XML file from %s...\n", url);

        f = fopen(file,"wb");
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
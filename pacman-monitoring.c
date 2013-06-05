/*************************************************************************\
*                  Alexandre Filgueira - Antergos                         *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Affero General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
*                                                                         *
\*************************************************************************/

#include <sys/inotify.h>
#include <limits.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

static void             /* Display information from inotify_event structure */
displayInotifyEvent(struct inotify_event *i)
{
    if (i->mask & IN_OPEN)        printf("Pacman executed");
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

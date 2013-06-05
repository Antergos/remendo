# Remendo

## How it is supposed to work

 * Thanks to inotify, pacman-monitoring knows when /usr/bin/pacman is opened. At this moment, remendo triggers itself.
 * Downloads a XML file from http://install.antergos.com/events.xml which contains the possible events to be fixed.
 	* The XML has the event ID, which is a number, the event name and description, and the URL where Remendo will download the script to fix the system once the user says yes to Remendo [more fields to be added]
 * Detects if the user has some pending events. Remendo should check if the user needs to fix the issue or if he already solved it manually.
 * Pauses the pacman process and show up an UI asking the user what to do.
 	* The user will have information on the screen of Event's name with a short description and URL to possible announcement of the fix.
 	* The user will have to choose between "Fixme the problem" or "I'm gonna do it manually" option.
 * If the user choose "I'm gonna do it manually", Remendo should mark the event as read and pacman should be reanudated.
 * If the user choose "Fixme the problem", Remendo will download the script provided in the XML URL field and execute this script.
 	* After finishind sucessfully the event fix, Remendo will mark it as read


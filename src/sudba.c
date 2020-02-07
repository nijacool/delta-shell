#include <stdlib.h>
#include "sudba.h"
#include <syslog.h>
#include <string.h>
#include <stdio.h>

// SUDBa main loop

int main(int argc, char *argv[])
{
	openlog("sudba", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); //Open logger
	char *string_pointer = sudba_read_line(0);
	puts(string_pointer);
	if (string_pointer != NULL) free(string_pointer); //Free only if the pointer is not null. 
	closelog();
  return EXIT_SUCCESS;
}

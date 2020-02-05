#include <stdlib.h>
#include "sudba.h"
#include <syslog.h>
#include <stdio.h>

// SUDBa main loop

int main(int argc, char *argv[])
{
	openlog("sudba", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); //RESOLVED! Why in the assignment you said "sudba" less the quotes????????????????????????????????
	char *r = sudba_read_line(0);
	//printf("r[0] is %c", r[0]); //testing purposes
	if (r != NULL) free(r); //Free only if the pointer is not null. 
	closelog();
	

  return EXIT_SUCCESS;
}

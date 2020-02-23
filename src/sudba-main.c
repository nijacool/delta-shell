#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <ctype.h> // included for isdigigt()
#include "susql-parser.h"
#include <unistd.h> // included to open a new file and store the port in directory
#include <sudba.h>

/* Port for the server */
static int PORT = 8000;

int sudba_initialize(int argn, char *argv[])
{
  if(argn > 2){
    // DZ: Your user does not know about argn!
	fprintf(stderr, "Error: argn is greater than 2.\n");
	return 1; 
	}

  // DZ: How do you know argv[1] exists? Have you checked? (No.)
  int i = atoi(argv[1]);
  if (i > 0) {
    // DZ: This message does not belong here
	printf("Success PORT = argv[1]: PORT = %i\n", i);
	PORT = i;
	}
  else {
   // DZ: Your user does not know about argv[1]!
	fprintf(stderr, "Error: argv[1] is not a positive integer number.\n");
	return 1; 
	}
  /* 2 */
	int j = chdir(SUDBA_WD);
	// DZ: When using perror(), name the offender, not the reason:
	// DZ: perror(SUDBA_WD)
	if (j != 0) { perror("Error when changing directory"); return 1; }
	// DZ: This message does not belong here
	if (j == 0) { printf("Success changing working directory: %s \n", SUDBA_WD);}
  /* 3 */
  	int ipid = getpid();
	FILE *fptr = fopen("sudba.pid", "w");
	// DZ: When using perror(), name the offender, not the reason:
	// DZ: perror("sudba.pid")
	if (fptr == NULL) { perror("Error: fptr is null"); return 1; }
	if ((fprintf(fptr, "%d\n", ipid)) < 0) { perror("Error writing to fptr"); return 1;}
	fclose(fptr);
  /* 4 */
  openlog("sudba", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog(LOG_INFO, "Started");
  closelog();

  return 0;
}

/* The main function; do not change */
int main(int argn, char *argv[])
{
  int status = sudba_initialize(argn, argv);
  if (status != 0) return EXIT_FAILURE;
  int parse_result = sudba_parse(0);
  
  return EXIT_SUCCESS;
}

/* Error reporting function */
void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, const char* s)
{
  fprintf(stderr, "400 %s\n", s);
}

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
  if(argn > 3){
	printf("%i",argn);
	stderr;
	printf("argn<=3"); 
	return 1; 
	}

  if ((atoi(argv[2]) > 0)) {
	printf("Success");
	PORT = atoi(argv[2]);
	}
  else {
	printf("argv[2]<=0");
	stderr;
	return 1; 
	}
	
  /* 2 */
	if (chdir(SUDBA_WD) == -1) { perror("Error when changing directory"); return 1;}
	printf(SUDBA_WD); //Ask DZ about this


  /* 3 */
  	getpid();
	FILE *fptr = fopen("sudba.pid", "w");
	if (fptr == NULL) { perror("Error, fptr is null"); return 1; }
	if ((fprintf(fptr, "%s %s", argv[1], argv[2])) < 0) { perror("Error writing fptr"); return 1;}
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

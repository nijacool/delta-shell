#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include "susql-parser.h"

/* Port for the server */
static int PORT = 8000;

int sudba_initialize(int argn, char *argv[])
{
  /* 1 */
  /* 2 */
  /* 3 */
  
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

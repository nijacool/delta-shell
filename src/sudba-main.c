#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include "sudba.h"
#include "susql-parser.h"

/* Port for the server */
static int PORT = 8000;

static bool sudba_initialize(int argn, char *argv[])
{
  /* 1 */
  if (argn > 2 || (argn == 2 && ((PORT = atoi(argv[1])) <= 0))) {
    fprintf(stderr, "Usage: %s [port-number > 0]\n", argv[0]);
    return false;
  }
  
  /* 2 */
  if (-1 == chdir(DB_WD)) {
    perror(DB_WD);
    return false;
  }
  
  /* 3 */
  FILE *pidfile = fopen(DB_PIDFILE, "w");
  if (!pidfile || fprintf(pidfile, "%d\n", getpid()) <= 0) {
    perror(DB_PIDFILE);
    return false;
  }
  fclose(pidfile);
  
  /* 4 */
  openlog(DB_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog(LOG_INFO, "Started");
  closelog();

  return true;
}

/* The main function; do not change */
int main(int argn, char *argv[])
{
  int status = sudba_initialize(argn, argv);
  if (status == false) return EXIT_FAILURE;
  
  // This lines fakes reading from a socket
  // As written, it reads from the standard input (that is, from the keyboard)
  int parse_result = sudba_parse(0);
  return EXIT_SUCCESS;
}

/* Error reporting function; do not change */
void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, const char* s)
{
  // Make some compilers happy
  scanner = 0;
  yyllocp = NULL;
  
  fprintf(stdout, HTTP_VER " 400 Bad Request %s\n\r", s);
}

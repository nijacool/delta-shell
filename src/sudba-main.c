#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <glob.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "susql-parser.h"
#include "sudba.h"

/* Port for the server */
static int PORT = 8000;

/* Names of the database tables, and their count */
char **sudba_tables = NULL;
int sudba_tab_count = 0;

static int sudba_initialize(int argn, char *argv[])
{
  /* 1 */
  if (argn > 2 || (argn == 2 && ((PORT = atoi(argv[1])) <= 0))) {
    fprintf(stderr, "Usage: %s [port-number > 0]\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  /* 2 */
  if (-1 == chdir(DB_WD)) {
    perror(DB_WD);
    return EXIT_FAILURE;
  }
  
  /* 3 */
  FILE *pidfile = fopen(DB_PIDFILE, "w");
  if (!pidfile || fprintf(pidfile, "%d\n", getpid()) <= 0) {
    perror(DB_PIDFILE);
    return EXIT_FAILURE;
  }
  fclose(pidfile);
  
  /* 4 */
  openlog(DB_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog(LOG_INFO, "Started");
  closelog();

  return EXIT_SUCCESS;
}

// Store database name in the global table
// The table, in general, does not have to be an array
static int sudba_store_table_name(char *name)
{

  // Your code goes here
  fprintf(stderr, "200 Found table %s\n", name);
  return EXIT_SUCCESS;
}

// Read table names from the working directory
static int sudba_read_tables()
{
  /* 1 */

  /* 2 */

  /* 3 */

  /* 4 */

  /* 5 */

  return EXIT_SUCCESS;
}

/* The main function; do not change */
int main(int argn, char *argv[])
{
  int status = sudba_initialize(argn, argv);
  if (status != EXIT_SUCCESS) return EXIT_FAILURE;
  
  status = sudba_read_tables();
  if (status != EXIT_SUCCESS) return EXIT_FAILURE;

  // This lines fakes reading from a socket
  // As written, it reads from the standard input (that is, from the keyboard)
  int parse_result = sudba_parse(0);
  return EXIT_SUCCESS;
}

/* Error reporting function; do not change */
void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, const char* s)
{
  fprintf(stderr, "400 %s\n", s);
}

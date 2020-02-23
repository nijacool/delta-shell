#ifndef SUDBA_H
#define SUDBA_H
#include <stdbool.h>

// Constants
#define DB_NAME "sudba"
#define HTTP_VER "HTTP/1.1"
#define DB_WD ("/tmp/" DB_NAME "/")
#define DB_PIDFILE (DB_NAME ".pid")
#define DB_SCHEMA_EXT ".frm"
#define DB_DATA_EXT   ".MYD"
#define DB_INDEX_EXT  ".MYI" // Not used

// General utilities
char *sudba_make_string(char *text);

// Database utilities
void sudba_lock(char *table);
void sudba_unlock(char *table);
bool sudba_exists(char *table);

// Database functions
bool sudba_drop_database(char *table);
#endif

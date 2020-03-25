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

// Data Types
typedef struct Column {
  enum {COL_INT, COL_FLOAT, COL_STR} type;
  short width;
  char *name;
} Column;

typedef struct Columns {
  int number;
  Column *declarations;
} Columns;

// General utilities
char *sudba_make_string(char *text);
void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

// Database utilities
void sudba_lock(char *table);
void sudba_unlock(char *table);
bool sudba_exists(char *table);

// Database functions
bool sudba_test(char *table, Columns columns);
bool sudba_drop_database(char *table);
bool sudba_create_database(char *table, Columns columns);
#endif

#ifndef SUDBA_H
#define SUDBA_H

#define DB_NAME "sudba"
#define DB_WD ("/tmp/" DB_NAME "/")
#define DB_PIDFILE (DB_NAME ".pid")
#define DB_SCHEMA_EXT ".su"
#define DB_DATA_EXT ".MYD"
#define DB_INDEX_EXT ".MYI"


char *makeString(char *text);

#endif

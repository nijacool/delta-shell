#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudba.h"

bool sudba_drop_database(char *table) {
  sudba_lock(table);

  bool status = true;
  if (!sudba_exists(table)) {
    fprintf(stdout, HTTP_VER " 404 Not Found %s\n\r", table);
    status = false;
  } else {  
    char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)];
    char data  [strlen(table) + sizeof(DB_DATA_EXT  )];
    sprintf(schema, "%s" DB_SCHEMA_EXT, table);
    sprintf(data  , "%s" DB_DATA_EXT  , table);
    
    if (unlink(schema) || unlink(data)) {
      fprintf(stdout, HTTP_VER " 500 Could Not Drop %s\n\r", table); 
      status = false;
    } else
      fprintf(stdout, HTTP_VER " 200 Deleted %s\n\r", table); 
  }
  
  sudba_unlock(table);
  free(table);
  return status;
}

bool sudba_create_database(char *table, Columns columns) {
  sudba_lock(table);

  bool status = true;

  // This loop is for testing. Please remove it before submitting
  /*
    for (int i = 0; i < columns.number; i++) {
    fprintf(stderr, "%i %i %s\n",
    columns.declarations[i].type,
    columns.declarations[i].width,
    columns.declarations[i].name);
    }
  */

  // Check if the table already exists. If it does, report error 412

  // Check if all column names are distinct. If not, report error 412

  // Create an empty .MYD file. If failed, report error 500
  
  // Create a .frm file according to the requirements.
  // If failed, delete the .MYD file, report error 500

  // Report success 201
  
  sudba_unlock(table);
  free(columns.declarations);
  free(table);
  return status;
}

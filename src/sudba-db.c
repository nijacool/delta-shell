#include <stdio.h>
#include <stdlib.h>
#include "sudba.h"

bool sudba_drop_database(char *table) {
  sudba_lock(table);

  bool status = true;
  if (sudba_exists(table) == false) {
    fprintf(stdout, HTTP_VER " 404 Not Found %s\n\r", table);
    status = false;
  } else {  
    // Your code goes here
    fprintf(stdout, HTTP_VER " 200 Deleted %s\n\r", table);
  }
  
  sudba_unlock(table);
  free(table);
  return status ;
}

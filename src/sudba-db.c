#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudba.h"

bool sudba_drop_database(char *table) {
  sudba_lock(table);
  bool status = true;
  if (sudba_exists(table) == false) {
    fprintf(stdout, HTTP_VER " 404 Not Found %s\n\r", table);
    status = false;
  } else {  
    // Your code goes here
	char frm[strlen(table)+strlen(DB_SCHEMA_EXT)+1];
	strcpy(frm,table); strcat(frm,DB_SCHEMA_EXT); 
	char MYD[strlen(DB_WD)+strlen(table)+strlen(DB_DATA_EXT)+1];
	strcpy(MYD,table); strcat(MYD,DB_DATA_EXT);
	int status_frm; 
	int status_MYD;
	status_frm = unlink(frm);
	status_MYD = unlink(MYD);

	if ((status_frm != 0) || (status_MYD != 0)) {
		fprintf(stdout, HTTP_VER " 400 Could not delete table %s\n\r", table); 
		status = false;
	}
	else {
    		fprintf(stdout, HTTP_VER " 200 Deleted %s\n\r", table); 
	}
  }
  
  sudba_unlock(table);
  free(table);
  return status ;
}

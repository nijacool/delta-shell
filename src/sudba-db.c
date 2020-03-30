#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudba.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
  bool have_duplicate = false;
  bool status = true;


  if (sudba_exists(table)) { 
	fprintf(stdout, HTTP_VER " 412 Precondition Failed %s already exists\n\r", table);
        status = false; 
  }
  else { //
  	for (int i = 0; i < columns.number; i++) {
		for (int j = i+1; j < columns.number; j++) {
			if (!strcmp(columns.declarations[i].name,columns.declarations[j].name)) {
					have_duplicate = true;
					break;
				}
		}
	}

  	if(have_duplicate == true) {  
		fprintf(stdout, HTTP_VER " 412 Precondition Failed duplicate column names in %s\n\r", table);
  	}
	else { ////
	  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)]; 
	  char data  [strlen(table) + sizeof(DB_DATA_EXT  )]; 
	  sprintf(schema, "%s" DB_SCHEMA_EXT, table);
	  sprintf(data  , "%s" DB_DATA_EXT  , table);
	  int myd = open(schema, O_CREAT|O_WRONLY, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	  if(myd < 0){  
		status = false; 
	  }
	  else {
		int frm = open(schema, O_CREAT|O_WRONLY, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
		if(frm < 0) { 
			remove(data); 
			status = false; 
		}
	 
	 	if (status == true) {
		  for (int i = 0; i < columns.number; i++) {
			write(frm,&columns.declarations[i].type,sizeof(int)); 
			write(frm,&columns.declarations[i].width,sizeof(short));
			short name_length = strlen(columns.declarations[i].name);
			write(frm,&name_length,sizeof(short)); 
			write(frm,columns.declarations[i].name,name_length); 
		    }
		close(frm);
       		close(myd);
		fprintf(stdout, HTTP_VER "201 created table %s\n\r", table); 
		}
	  }

	  if (status == false) {
		fprintf(stdout, HTTP_VER "500 Internal Server Error table cannot be created. %s\n\r", table);
	  }



	  } ////
  } //

  sudba_unlock(table);
  free(columns.declarations);
  free(table); 
  return status;
  }

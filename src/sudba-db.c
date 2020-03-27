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

bool sudba_test(char *table){ //this function is for testing. will delete
  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)]; 
  //char data  [strlen(table) + sizeof(DB_DATA_EXT  )]; 
  sprintf(schema, "%s" DB_SCHEMA_EXT, table);
  //sprintf(data  , "%s" DB_DATA_EXT  , table);
  int s_read = open(schema, O_RDONLY);
	printf("s_read value: %i\n",s_read);
  char *buffer = my_malloc(36);
  printf("%zd\n", read(s_read, buffer,36));
	puts(buffer);
  return false;

}

bool sudba_create_database(char *table, Columns columns) {

  sudba_lock(table);
  bool have_duplicate = false;
  bool status = true;


  if (sudba_exists(table)) { 
	fprintf(stdout, HTTP_VER " 412 Precondition Failed %s already exists\n\r", table);
        status = false; //QQ: does status become false at the event of an error?
  }


  else {
  	for (int i = 0; i < columns.number; i++) {
		for (int j = i+1; j < columns.number; j++) {
			//if (have_duplicate = true) { break; } //QQ: What about this extra if to break everytime? or i = columns.number
			if (!strcmp(columns.declarations[i].name,columns.declarations[j].name)) {
					have_duplicate = true;
					break;
				}
		}
	}
  	if(have_duplicate == true) { //QQ: concern that the loop does not end when needed. the loop would still run, but once the first duplicate is found, it should end. 
		fprintf(stdout, HTTP_VER " 412 Precondition Failed duplicate column names in %s\n\r", table);
  	}
  }


  if (have_duplicate == false) { 
  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)]; 
  char data  [strlen(table) + sizeof(DB_DATA_EXT  )]; 
  sprintf(schema, "%s" DB_SCHEMA_EXT, table);
  sprintf(data  , "%s" DB_DATA_EXT  , table);
  int myd = open(schema, O_CREAT|O_WRONLY, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
  close(myd);
  int frm = open(schema, O_CREAT|O_WRONLY, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
  if(myd < 0){ 
	remove(data); 
  	fprintf(stdout, HTTP_VER "500 Internal Server Error table cannot be created. %s\n\r", table);
	status = false; //QQ: does status become false at the event of an error?
  }

  else {
	if(frm < 0){ //Documentation says if creating fails, it won't open, so we don't need to close
		remove(schema);
		remove(data); 
      		fprintf(stdout, HTTP_VER "500 Internal Server Error table cannot be created. %s\n\r", table);
		status = false; //QQ: does status become false at the event of an error?
	}
  }

  for (int i = 0; i < columns.number; i++) {
	write(frm,&columns.declarations[i].type,sizeof(int)); 

	if (columns.declarations[i].type != 2) {
	write(frm, 0, sizeof(short));
	}
	else {
	write(frm,&columns.declarations[i].width,sizeof(short)); //Writes the type as a SHORT binary number QQ: 0 for non-string columns? Is this a condition sql provides or we must implement an if?
	} 
	
	int name_length = strlen(columns.declarations[i].name);
	write(frm,&name_length,sizeof(short)); //Writes the length of the name as a SHORT binary number //QQ: Unsure how to do this. Need to cast to int?
	write(frm,&columns.declarations[i].name,name_length*sizeof(char)); //QQ: *sizeof(char)?
    }
	close(frm);
  

 

    } 

  // Check if the table already exists. If it does, report error 412

  // Check if all column names are distinct. If not, report error 412

  // Create an empty .MYD file. If failed, report error 500

  // Create a .frm file according to the requirements.
  // If failed, delete the .MYD file, report error 500

  // Report success 201 
  fprintf(stdout, HTTP_VER "201 created table %s\n\r", table); //QQ: right report?
  sudba_unlock(table);
  free(columns.declarations);
  free(table); //QQ: pointer error in sudba-main
  return status;
  }

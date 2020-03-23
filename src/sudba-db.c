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
  if (sudba_exists(table)) { fprintf(stdout, HTTP_VER " 412 Precondition Failed table already exists %s\n\r", table);} //is this the right error?
	else {
  for (int i = 0; i < columns.number; i++) {
		for (int j = i+1; j < columns.number; j++) { //I think there is a better way instead of columns.number. Maybe sizeOf(column_names)
			if (!strcmp(columns.declarations[i].name,columns.declarations[j].name)) {
					printf("FOR TESTING!: The duplicates are [%i] %s [%i] %s\n" ,i,columns.declarations[i].name,j,columns.declarations[j].name);
					fprintf(stdout, HTTP_VER " 412 Precondition Failed duplicate column names in table %s\n\r", table);
					//break? //question for DZ: why did the other stuff not get repeated?
				}
		}
	}
  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)]; //copied from sudba-utils.c
  char data  [strlen(table) + sizeof(DB_DATA_EXT  )];
  sprintf(schema, "%s" DB_SCHEMA_EXT, table);
  sprintf(data  , "%s" DB_DATA_EXT  , table);

  //added by Cristian (open file, and check if they can be created/open or not)
  FILE *myd = fopen(schema, "w");
  fclose(myd);//Since we are just creating an empty .myd.
  if(myd == NULL){
	//delete myd
	if(remove(myd)==0){
		fprint(stdout,HTTP_VER, "500 Internal Server Error table cannot be created." "%s\n\r", table);
	};
	status = false;
  }else{
	FILE *myd = fopen(data, "w");
	if(frm == NULL){
		fclose(frm);
		if((remove(frm)==0)&&(remove(mys)==0)){ //delete frm and myd
			fprint(stdout,HTTP_VER, "500 Internal Server Error table cannot be created." "%s\n\r", table);
	};
	status = false;
  }
	//end 

  for (int i = 0; i < columns.number; i++) {
	write(1, &columns.declarations[i].type, 4); //not sure if this is right
	/*if (columns.declarations[i].type != 2) {
	write(1, columns.declarations[i].width, 4);
	}
	else {
	write(1, columns.declarations[i].width, 4); //0 for non string columns
	} */
	int strl = strlen(columns.declarations[i].name);
	write(1, &strl, 4); //not sure if right. writes the length of the name
	write(1, columns.declarations[i].name, strl);
    }
	}
	

  // This loop is for testing. Please remove it before submitting
  
    for (int i = 0; i < columns.number; i++) {
    fprintf(stderr, "Start: %i %i %lu %s\n",
    columns.declarations[i].type,
    columns.declarations[i].width,
    strlen(columns.declarations[i].name), //CHANGED WIDTH to name
    //columns.declarations[i].length, //ADDED FOR DEBUGGING
    columns.declarations[i].name);
    }
  

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

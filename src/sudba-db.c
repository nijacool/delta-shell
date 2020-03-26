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

bool sudba_test(char *table, Columns columns){ //this function is for testing. will delete
  int frm = open("./test_column.MYD", O_CREAT|O_RDWR, 0666);


  for (int i = 0; i < columns.number; i++) {
    //char type_buffer[4]; //Q: is this okay as buffer to write? since the hw asked us for it to be an integer binary number, but ths buffer would be an integer, in the form of a char written.
    //char width_buffer[2]; //Q: same with the width buffer, which wants a short binary number
    //char name_length_buffer[2];
    //char name_buffer[32];
    //sprintf(type_buffer, "%d", columns.declarations[i].type);
    write(frm,&columns.declarations[i].type,sizeof(int));//sizeof(int) or size of(the type)?)
    //sprintf(width_buffer, "%d", columns.declarations[i].width);
    write(frm,&columns.declarations[i].width,sizeof(short));
    //sprintf(name_length_buffer, "%lu", strlen(columns.declarations[i].name));
	//int temp = static_cast<int>(strlen(columns.declarations[i].name));
    //write(frm,(int)strlen(columns.declarations[i].name),sizeof(short)); //Q: how do you determine the size of name_length so that it prints the exact bytes?
    //strcpy(name_buffer,columns.declarations[i].name);
    write(frm,&columns.declarations[i].name,strlen(columns.declarations[i].name)*sizeof(char)); //Q: Do we even need a name buffer since we can just write the name pointer directly?
  }
  close(frm);
  return false;

}

bool sudba_create_database(char *table, Columns columns) {

  sudba_lock(table);
  bool have_duplicate = false;
  bool status = true;


  if (sudba_exists(table)) { //Checks if table exists, if it does, print error QQ: Do we need to change status to false on errors?
	fprintf(stdout, HTTP_VER " 412 Precondition Failed %s already exists\n\r", table);
  }


  else {
  	for (int i = 0; i < columns.number; i++) {
		for (int j = i+1; j < columns.number; j++) {
			//if (have_duplicate = true) { break; } //QQ: What about this extra if to break everytime?
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
		status = false; 
	}
  }

  for (int i = 0; i < columns.number; i++) {
	write(frm,&columns.declarations[i].type,sizeof(int)); 
	write(frm,&columns.declarations[i].width,sizeof(short)); //Writes the type as a SHORT binary number QQ: 0 for non-string columns? Is this a condition sql provides or we must implement an if?
	//write(frm,(int)strlen(columns.declarations[i].name),sizeof(short)); //Writes the length of the name as a SHORT binary number //QQ: Unsure how to do this. Need to cast to int?
	write(frm,&columns.declarations[i].name,strlen(columns.declarations[i].name)*sizeof(char));
	/*if (columns.declarations[i].type != 2) {
	write(1, columns.declarations[i].width, 4);
	}
	else {
	write(1, columns.declarations[i].width, 4); //0 for non string columns
	} */
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

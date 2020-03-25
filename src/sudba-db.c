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
  int frm = open("test_column.MYD", O_WRONLY | O_APPEND);


  for (int i = 0; i < columns.number; i++) {
    char type_buffer[4]; //Q: is this okay as buffer to write? since the hw asked us for it to be an integer binary number, but ths buffer would be an integer, in the form of a char written.
    char width_buffer[2]; //Q: same with the width buffer, which wants a short binary number
    char name_length_buffer[2];
    char name_buffer[32];
    sprintf(type_buffer, "%d", columns.declarations[i].type);
    write(frm,type_buffer,1);
    sprintf(width_buffer, "%d", columns.declarations[i].width);
    write(frm,width_buffer,1);
    sprintf(name_length_buffer, "%lu", strlen(columns.declarations[i].name));
    write(frm,name_length_buffer,1); //Q: how do you determine the size of name_length so that it prints the exact bytes?
    strcpy(name_buffer,columns.declarations[i].name);
    write(frm,columns.declarations[i].name,strlen(columns.declarations[i].name)); //Q: Do we even need a name buffer since we can just write the name pointer directly?
  }
  close(frm);
  return false;

}

bool sudba_create_database(char *table, Columns columns) {
  sudba_lock(table);

  bool status = true;
  if (sudba_exists(table)) {
	fprintf(stdout, HTTP_VER " 412 Precondition Failed %s already exists\n\r", table);
} //C: JUST NEED TO CONFIRM THAT fprintf(stdout, HTTP_VER " 412 Precondition Failed %s already exists\n\r", table); IS THE RIGHT ERROR TO REPORT WHEN sudba_exists(table) is true, meaning table already exists
  else {
  for (int i = 0; i < columns.number; i++) {
		for (int j = i+1; j < columns.number; j++) { //C: JUST NEED TO CONFIRM THE CODE FOR CHECKING DUPLICATES IS CORRECT.
			if (!strcmp(columns.declarations[i].name,columns.declarations[j].name)) {
					printf("FOR TESTING!: The duplicates are [%i] %s [%i] %s\n" ,i,columns.declarations[i].name,j,columns.declarations[j].name); //Q: DURING MAKE TEST, DESPITE BEING MULTIPLE DUPLICATES, THIS TEST PRINT MESSAGE ONLY PRINTS ONCE. WHY IS THAT?
					fprintf(stdout, HTTP_VER " 412 Precondition Failed duplicate column names in %s\n\r", table);
					//Q: should we break by returning false? if there is one duplicate, then that should be it.
				}
		}
	}
} //Q: IF THERE IS DUPLICATES, THEN AN ASSUMPTION IS MADE THAT THE TABLES SHOULD NO BE CREATED. SHOULD THERE WWE RETURNING FALSE BEFORE CONTINUING?
  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)]; //copied from sudba-utils.c SCHEMA IS FRM
  char data  [strlen(table) + sizeof(DB_DATA_EXT  )]; //DATA IS MYD
  sprintf(schema, "%s" DB_SCHEMA_EXT, table);
  sprintf(data  , "%s" DB_DATA_EXT  , table);

  //added by Cristian (open file, and check if they can be created/open or not)
  int myd = open(schema, O_CREAT , O_WRONLY | O_APPEND);////////
  int frm = open(schema, O_CREAT , O_WRONLY | O_APPEND); //Q: Is O_WRONLY | O_APPEND the right parameters? without O_APPEND, it seems like it will just overwrite the data.
  if(myd < 0){ //AKA it failed.
	//delete myd
	if(remove(data)==0){ //changed "myd" to "data", since myd is a file but the function takes a char
    fprintf(stdout, HTTP_VER "500 Internal Server Error table cannot be created. %s\n\r", table);
	};
	//Q: what if remove was unsuccessful (e.g. remove(data) returns -1)?
	status = false;
  }

else{
	if(frm < 0){
		//close(frm); //i guess this right? i'm not sure though.
		status = false; //false because if frm is less than 0, meaning the table is not created
		if((remove(schema)==0)&&(remove(data)==0)){ //delete frm and myd
		//what if remove was unsuccessful (e.g. remove(data) returns -1)?
      fprintf(stdout, HTTP_VER "500 Internal Server Error table cannot be created. %s\n\r", table);
	}}
  }
	//end
  //FOR TESTING PURPOSES!

    //FOR TESTING PURPOSES!
  for (int i = 0; i < columns.number; i++) {
    //char *temp = columns.declarations[i].type;
	write(frm, "hey", 4); //not sure if this is right
	/*if (columns.declarations[i].type != 2) {
	write(1, columns.declarations[i].width, 4);
	}
	else {
	write(1, columns.declarations[i].width, 4); //0 for non string columns
	} */
	//int strl = strlen(columns.declarations[i].name);
	//write(1, &strl, 4); //not sure if right. writes the length of the name
	//write(1, columns.declarations[i].name, strl);
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

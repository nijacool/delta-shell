#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
  // Free table name
  free(table);
  return status;
}

/*
  Read SuSQL schema for the table.
*/
static bool read_schema (char *table, Columns *columns) { 
  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)];
  sprintf(schema, "%s" DB_SCHEMA_EXT, table);
  int s = open(schema, O_RDONLY);
  if (s == -1) { 
	return false;
  } 
  int type;
  short width;
  short name_length;
  char *name;
  int w = 1;
  columns->declarations = NULL;
  columns->number = 0;
  while(w > 0){ 
	if ((read(s, &type, sizeof(type))) <= 0){ 
		break; //FLEX will handle catches
		}
	columns->declarations = my_realloc(columns->declarations,(columns->number+1)*sizeof(Column));
	w = read(s, &width, sizeof(width));
	w = read(s, &name_length, sizeof(name_length));
	name = my_malloc(name_length+1);
	w = read(s, name, name_length);
	name[name_length] = '\0';
	columns->declarations[columns->number].type = type; 
	columns->declarations[columns->number].width = width;
	columns->declarations[columns->number].name = name; 
	columns->number = (columns->number + 1); 
	}  
  
  close(s);
  return true;
}

/*
  Write SuSQL schema for the table.
*/
static bool write_schema (char *table, Columns columns) {
  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)];
  sprintf(schema, "%s" DB_SCHEMA_EXT, table);

  // Create a .frm file with permissions -rw-r--r--
  int schema_file = creat(schema, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (schema_file < 0)
    return false;

  // Write the schema into the file
  for (int i = 0; i < columns.number; i++) {
    short name_length = strlen(columns.declarations[i].name);
    if (-1 == write(schema_file, &columns.declarations[i].type,
		    sizeof columns.declarations[i].type) ||
	-1 == write(schema_file, &columns.declarations[i].width,
		    sizeof columns.declarations[i].width) ||
	-1 == write(schema_file, &name_length, sizeof name_length) ||
	-1 == write(schema_file, columns.declarations[i].name,
		    name_length)) {
      close(schema_file);
      return false;
    }
  }
  
  return -1 != close(schema_file);
}
  
bool sudba_create_database(char *table, Columns columns) {
  sudba_lock(table);
  bool status = true;

  // Check if the table already exists. If it does, report error 412
  if (sudba_exists(table)) {
    status = false;
  } else {
    // Check if all column names are distinct. If not, report error 412
    for (int i = 0; i < columns.number; i++)
      for (int j = i + 1; j < columns.number; j++)
	if(!strcmp(columns.declarations[i].name, columns.declarations[j].name)) {
	  status = false;
	  break;
	}
  }

  if (!status)
    fprintf(stdout, HTTP_VER " 412 Duplicate\n\r");
  else {
    char data[strlen(table) + sizeof(DB_DATA_EXT)];
    sprintf(data, "%s" DB_DATA_EXT, table);

    // Create an empty .MYD file. If failed, report error 500
    int data_file = creat(data, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    // Create a .frm file according to the requirements.
    status = (data_file >= 0)
      && (-1 != close(data_file))
      &&  write_schema(table, columns);
    
    if (!status) {
      // If failed, delete the .MYD file, report error 500
      unlink(data);
      fprintf(stdout, HTTP_VER " 500 Internal Server Error %s\n\r", table);
    } else
      // Report success 201
      fprintf(stdout, HTTP_VER " 201 Created %s\n\r", table);
  }
  
  sudba_unlock(table);
  // Free strings
  for(int i = 0; i < columns.number; i++ )
    if (columns.declarations[i].type == COL_STR)
      free(columns.declarations[i].name);
  free(columns.declarations);
  // Free table name
  free(table);
  return status;
}

bool sudba_insert_into_database(char *table, Values values)
{
  sudba_lock(table);
  bool status = true;
  // 1. Check if the table already exists. If it does not exist, report error 412
  if(!sudba_exists(table)){
	fprintf(stdout, HTTP_VER " 412 Precondition Failed\n");
	status = false;
  }
  // 2. Read the table schema from the .frm file
  // If the function fails, report error 500
  Columns columns;
  if(status == true){
	status = read_schema(table, &columns);
		if (status == false){
			fprintf(stdout, HTTP_VER " read_schema 500 Internal Server Error\n");
		}
  }

  // 3. Compare the passed values to the columns. The number and types must match
  // If they do not, report error 400
  if (status == true){
	  for (int i = 0; i < values.number; i++ ) {
		  if (values.values[i].type != columns.declarations[i].type) {
				fprintf(stdout, HTTP_VER " 400 Invalid Request %s\n\r", table);
				status = false; 
				break;
			}
		}
  }
  // 4. Append the values, in the binary form, at the end of the .MYD file
  // without separators or terminators.
  //    Strings shall be written as left-justified, 0-padded character arrays
  //    of the length "width+1", as specified in the column definitions, trimmed
  //    if needed.
  //    For example, string "Hello" shall be written into column char(10) as
  //    "Hello\0\0\0\0\0" (six zeros, including the trailing terminator!).
  //    The same string shall be written into column char(2) as "He" (trimmed).
  // If writing fails, report error 500

  if (status == true){
  	char data[strlen(table) + sizeof(DB_DATA_EXT)];
  	sprintf(data, "%s" DB_DATA_EXT, table);
  	int o = open(data, O_WRONLY|O_APPEND);
	for (int i = 0; i < values.number; i++){
		if (o == -1){
			status = false;
			fprintf(stdout, HTTP_VER " Test 500 Internal Server Error\n");//QQ: right error?
			break;//QQQ: break?
		}
		else{//

			switch(values.values[i].type) {////
				case(COL_INT) :
					if (write(o, &values.values[i].value.int_val, sizeof(values.values[i].value.int_val)) < 0){ 
						fprintf(stdout, HTTP_VER " int 500 Internal Server Error\n"); 
						status = false; 
						}
					else { //debugging
						printf("int_val = %i\n", values.values[i].value.int_val);
					}
					break;
				case(COL_FLOAT) :
					if (write(o, &values.values[i].value.float_val, sizeof(values.values[i].value.float_val)) < 0){
						fprintf(stdout, HTTP_VER " float 500 Internal Server Error\n"); 
						status = false;
						}
					else { //debugging
						printf("float_val = %f\n", values.values[i].value.float_val);
					}
					break;
				case(COL_STR) : {
					char temp[columns.declarations[i].width+1];
					memset(temp,'\0',columns.declarations[i].width+1);
					strncpy(temp,values.values[i].value.string_val,columns.declarations[i].width);
					printf("\n\nDEBUGGING:\n");
					for (int j = 0; j<sizeof(temp);j++){
						if (temp[j] == '\0'){
							printf("temp[%i] = null!\n",j);
						}
						else{
							printf("temp[%i] = %c \n",j,temp[j]);
						}
						} printf("\n\n");//debugging
					if (write(o, temp, sizeof(temp)) < 0){
						fprintf(stdout, HTTP_VER " str 500 Internal Server Error\n"); 
						status = false;
					}
					break;
				}			
				}////
				
		}//
		
	} close(o);
  }
  // Report success 200
  if(status == true){  
  	fprintf(stdout, HTTP_VER " 200 Inserted into %s\n\r", table);
  }
  sudba_unlock(table);
  // Free strings
  for(int i = 0; i < values.number; i++ )
    if (values.values[i].type == COL_STR)
      free(values.values[i].value.string_val);
  free(values.values);

  // Free table name
  free(table);
  return status;
}

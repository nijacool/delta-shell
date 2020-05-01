#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sudba.h"

bool sudba_drop_database(char *table, FILE* response) {
  sudba_lock(table);

  bool status = true;
  if (!sudba_exists(table)) {
    fprintf(response, HTTP_VER " 404 Not Found %s\n\r", table);
    status = false;
  } else {  
    char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)];
    char data  [strlen(table) + sizeof(DB_DATA_EXT  )];
    sprintf(schema, "%s" DB_SCHEMA_EXT, table);
    sprintf(data  , "%s" DB_DATA_EXT  , table);
    
    if (unlink(schema) || unlink(data)) {
      fprintf(response, HTTP_VER " 500 Could Not Drop %s\n\r", table); 
      status = false;
    } else
      fprintf(response, HTTP_VER " 200 Deleted %s\n\r", table); 
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

  // Open the .frm file for reading
  int schema_file = open(schema, O_RDONLY);
  if (schema_file < 0) {
    return false;
  }

  // Initialize the schema
  int count = 0;
  columns->declarations = NULL;
  
  // Read the schema from the file
  int type;
  while (sizeof type == read(schema_file, &type, sizeof type)) {
    columns->declarations = my_realloc(columns->declarations,
				       (count + 1) * sizeof(Column));
    columns->declarations[count].type = type;
    short name_length;    
    if (-1 == read(schema_file, &columns->declarations[count].width,
		   sizeof columns->declarations[count].width) ||
	-1 == read(schema_file, &name_length, sizeof name_length)) {
      // Roll back
      for (int i = 0; i < count; i++)
	free(columns->declarations[i].name);
      free(columns->declarations);
      close(schema_file);
      return false;
    }
    columns->declarations[count].name = my_malloc(name_length + 1);
    if (-1 == read(schema_file, columns->declarations[count].name,
			    name_length)) {
      // Roll back
      for (int i = 0; i <= count; i++)
	free(columns->declarations[i].name);
      free(columns->declarations);
      close(schema_file);
     return false;
    }
    columns->declarations[count].name[name_length] = 0;
    count++;
  }
  columns->number = count;
  
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
  
bool sudba_create_database(char *table, Columns columns, FILE* response) {
  sudba_lock(table);
  bool status = true;

  // Check if the table already exists. If it does, report error 412
  if (sudba_exists(table)) {
    fprintf(response, HTTP_VER " 412 Duplicated %s\n\r", table); 
    status = false;
  } else {
    // Check if all column names are distinct. If not, report error 412
    for (int i = 0; i < columns.number; i++)
      for (int j = i + 1; j < columns.number; j++)
	if(!strcmp(columns.declarations[i].name, columns.declarations[j].name)) {
	  status = false;
	  break;
	}
    if (!status)
      fprintf(response, HTTP_VER " 400 Bad Request\n\r");
  }

  if (status) {
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
      fprintf(response, HTTP_VER " 500 Internal Server Error %s\n\r", table);
    } else
      // Report success 201
      fprintf(response, HTTP_VER " 201 Created %s\n\r", table);
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

// Helper function for writing data into an existing table
static bool write_data(char *table, Values values, Columns columns)
{
  // 4. Append the values, in the binary form, at the end of the .MYD file
  // without separators or terminators.
  char data[strlen(table) + sizeof(DB_DATA_EXT)];
  sprintf(data, "%s" DB_DATA_EXT, table);
  
  // Open the .MYD file for appending
  int data_file = open(data, O_WRONLY | O_APPEND);
  if (data_file < 0)
    return false;
  
  for(int i = 0; i < values.number; i++ ) {
    Value val = values.values[i];
    void *ptr = NULL, *col = NULL;
    size_t count = 0;
    
    // Prepare for writing
    switch(val.type) {
    case COL_INT:
      ptr = &val.value.int_val;
      count = sizeof(int);
      break;
    case COL_FLOAT:
      ptr = &val.value.float_val;
      count = sizeof(float);
      break;
    case COL_STR:
      {
//    Strings shall be written as left-justified, 0-padded character arrays
//    of the length "width+1", as specified in the column definitions, trimmed
//    if needed.
//    For example, string "Hello" shall be written into column char(10) as
//    "Hello\0\0\0\0\0" (six zeros, including the trailing terminator!).
//    The same string shall be written into column char(2) as "He" (trimmed).
	count = columns.declarations[i].width + 1;
	ptr = col = my_malloc(count);
	bzero(col, count);
	memcpy(col, val.value.string_val, count -1);
      }
      break;
    }
    
    // Attempt to write
    if(count != write(data_file, ptr, count)) {
	if (col) free(col); 
		close(data_file);
	
      return false;
    }
  }
    
  // If writing fails, report error 500
    if (-1 == close(data_file))
    	return false;

  return true;
}

bool sudba_insert_into_database(char *table, Values values, FILE* response)
{
  bool status = true;
  sudba_lock(table);

  // 1. Check if the table already exists. If it does, report error 412
  if (!sudba_exists(table)) {
    fprintf(response, HTTP_VER " 404 Not Found %s\n\r", table);
    status = false;
  }

  Columns columns;
  if(status) {
    // 2. Read the table schema from the .frm file
    // If the function fails, report error 500
    status = read_schema (table, &columns);
  }

  if (status) {
    // 3. Compare the passed values to the columns.
    // The number and types must match    
    if (columns.number != values.number)
      status = false;
    else
      for(int i = 0; i < values.number; i++) {
	if (values.values[i].type != columns.declarations[i].type) {
	  status = false;
	  break;
	}      
      }
    if (!status)
      // If they do not, report error 400
      fprintf(response, HTTP_VER " 400 Bad Request\n\r");
  }

  if (status) {
    if (write_data(table, values, columns))
      // Report success 200
      fprintf(response, HTTP_VER " 200 Inserted into %s\n\r", table);
    else
      fprintf(response, HTTP_VER " 500 Internal server error\n\r");
  }
  
  // Cleanup
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

bool sudba_select(QualifiedColumns qcolumns, Tables tables, void *where, FILE* response)
{
  printf("\n"); //DEBUGGING
  bool status = true;

  // Much locking needed!
  for(int i = 0; i < tables.number; i++) {
    sudba_lock(tables.values[i]);
    printf("DEBUGGING tables.values[%i]: ", i); //DEBUGGING
    puts(tables.values[i]); //DEBUGGING
    printf("\n");
  }
  for (int j = 0; j < qcolumns.number; j++) { //DEBUGGING
	if (qcolumns.values[j].column == NULL) { 
		printf("DEBUGGING qcolumns.values[%i].column: NULL\n", j); 
	}
	else {
		printf("DEBUGGING qcolumns.values[%i].column: ", j);
		puts(qcolumns.values[j].column); 
		printf("\n");
	}
	if (qcolumns.values[j].table == NULL) { //DEBUGGING
		printf("DEBUGGING qcolumns.values[%i].table: NULL\n", j);
	}
	else {
		printf("DEBUGGING qcolumns.values[%i].table: ", j);
		puts(qcolumns.values[j].table); 
		printf("\n");
	}
    
  }


  
  // Will impement only for tables.number == 1
  // And qcolumns.number == 1
  // And qcolumns.values[0].table == NULL
  // And qcolumns.values[0].column == NULL

  //---------------------------------------------------
  if ((tables.number == 1) && (qcolumns.number == 1) && (qcolumns.values[0].table == NULL) && (qcolumns.values[0].column == NULL)) { //if 
	if (sudba_exists(tables.values[0])) {
		printf("Table name: %s\n", tables.values[0]);//DEBUGGING
		Columns rsc; //rsc = read_schema_column
		status = read_schema(tables.values[0], &rsc);
		if (status == true) {
		fprintf(response, HTTP_VER " 200 Success\n\r\n\r");
		int done = 1;
		char data  [strlen(tables.values[0]) + sizeof(DB_DATA_EXT  )];
  		sprintf(data, "%s" DB_DATA_EXT  , tables.values[0]);
		int data_file = open(data, O_RDONLY);
		fprintf(response, "%s | %s | %s | %s", rsc.declarations[0].name, rsc.declarations[1].name, rsc.declarations[2].name, rsc.declarations[3].name);//debugging
		while(done > 0) { //while done > 0
			fprintf(response, "\n");
			for (int k = 0; k < rsc.number; k++) {//for k
				int in;
				float fl;
				//dz did it like this
  				if (data_file < 0) { //QQ: neccessary?
					printf("OPEN FAIL!\n");//DEBUGGING
    						//return false;
  				}
				else {//else
					switch(rsc.declarations[k].type) {
						case COL_INT:
							done = read(data_file, &in, sizeof(int));
							if (!(done > 0)) { 
								break;
							}
							printf("int: %i\n", in);
							fprintf(response, "%i\t", in);
							break;
						case COL_FLOAT:
							done = read(data_file, &fl, sizeof(float));
							if (!(done > 0)) { 
								break;
							}
							printf("float: %f\n", fl);
							fprintf(response, "%f\t", fl);
							break;
						case COL_STR:
						{
							char buf[rsc.declarations[k].width+1];
							done = read(data_file, &buf, sizeof(char)*rsc.declarations[k].width+1);
							if (!(done > 0)) { 
								break;
							}
							printf("str: %s\n\nrsc.number.declarations[%i].width = %i", buf, k, rsc.declarations[k].width);
							fprintf(response, "%s\t", buf);
							
							
							for (int z = 0; z < sizeof(buf); z++) {
								if (buf[z] == '\0') {
									printf("buf[%i] is null\n", z);
								}
								else {
									printf("buf[%i]: %c\n", z, buf[z]);
								}
							}
							
							break;
						}
					}
				}//else
			}//for k
		}//while done > 0
		}//if schema == false
		else{
			fprintf(response, HTTP_VER " 500 Internal Server Error \n\r");
		}
		
	}
	else {
		status = false;
		fprintf(response, HTTP_VER " 404 Not Found\n\r");
	}
	}//if
/*

  for (int i = 0; i < tables.number; i++) {//1
	if (sudba_exists(tables.values[i])) {//2
  		for (int j = 0; j < qcolumns.number; j++) {//3
			if (qcolumns.values[j].table == NULL && qcolumns.values[0].column == NULL) { //QQ: do we need the other 2 ands
				Columns rsc; //rsc = read_schema_column
				status = read_schema(tables.values[i], &rsc);
				if (status == false) {
					fprintf(response, HTTP_VER " 500 Internal Server Error %s\n\r", tables.values[i]);
					break;
				}
				else {
					fprintf(response, HTTP_VER " 200 Success\n\r\n\r"); //for client or server? //delete tables.values[i] later?				
  					char data  [strlen(tables.values[i]) + sizeof(DB_DATA_EXT  )];
  					sprintf(data, "%s" DB_DATA_EXT  , tables.values[i]);
					int data_file = open(data, O_RDONLY);
					for (int k = 0; k < rsc.number; k++) {//4
						//dz did it like this
						int in;
						float fl;
						size_t count = 0;
						//dz did it like this
						fprintf(response, "%s | ", rsc.declarations[k].name);
  						if (data_file < 0) { //QQ: neccessary?
							printf("OPEN FAIL!\n");//DEBUGGING
    							//return false;
  						}
						else {
							**switch(rsc.declarations[k].type) {
								case COL_INT:
									read(data_file, &in, sizeof(int));
									printf("int: %i\n", in);
									break;
								case COL_FLOAT:
									read(data_file, &fl, sizeof(float));
									printf("float: %f\n", fl);
									break;
								case COL_STR:
									{
									char *buf = malloc(sizeof(char) * rsc.declarations[k].width+1);
									read(data_file, buf, rsc.declarations[k].width+1);
									//char buf[rsc.declarations[k].width+1];
									//bzero(buf, sizeof(buf));
									//read(data_file, &buf, sizeof(buf));
									//buf[sizeof(buf)] = '\0';
									printf("str: %s\n", buf);
									break;
									}
							}**
							//while (done > 0) { //while(!done) -----------
								

								done = read(data_file, &m_name, sizeof(m_name));
								printf("done: %i\n", done);
								m_name[strlen(m_name)] = '\0';
								done = read(data_file, &m_age, sizeof(m_age));
								printf("done: %i\n", done);
								done = read(data_file, &m_gender, sizeof(m_gender));
								printf("done: %i\n", done);
								m_gender[strlen(m_gender)] = '\0';
								done = read(data_file, &m_gpa, sizeof(m_gpa));
								printf("done: %i\n", done);
								printf("PRINTING: sizeof[k = %i] width: %i, name: %s type: %lu, type: %u \n",k,rsc.declarations[k].width, rsc.declarations[k].name, sizeof(rsc.declarations[k].type), rsc.declarations[k].type);
								printf("Magic variables: m_name: %s, m_age: %i, m_gender: %c, m_gpa: %f\n", m_name, m_age, m_gender[0], m_gpa);
								//puts(data_buffer);
								printf("\n");
								//memset(data_buffer, 0, sizeof(data_buffer));

							}
						}
					}//4
					fprintf(response, "\n");
					
				}
				
			}
  		}//3
	}//2
	else {
		status = false;
		fprintf(response, HTTP_VER " 404 Not FounDEBUGGINGSUDBA_EXIST FAILEDd %s\n\r", tables.values[i]);
		break;
		}	
  }//1*/
  //---------------------------------------------------

  // 0. Use file response instead of stdout for reporting!
  // 1. Check if the table exists; if it does not, report 404 Not Foundx
  // 2. Read the table schema
  // 3. Print HTTP_VER " 200 Success\n\r\n\r" (yes, a blank line)
  // 4. Print all column names on the same line, separated by vertical bars | 
  // 5. For each row in the data file, print column data in ASCII
  //    (not binary) format on one line per row, separated by vertical bars |
  
  // Cleanup
  for(int i = 0; i < tables.number; i++) {
    sudba_unlock(tables.values[i]);
  }
  for(int i = 0; i < qcolumns.number; i++){
    free(qcolumns.values[i].table);
    free(qcolumns.values[i].column);
  }
  
  for(int i = 0; i < tables.number; i++) {
    free(tables.values[i]);
  }

  free(qcolumns.values);
  free(tables.values);
  return true;  
}

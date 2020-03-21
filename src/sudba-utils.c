#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sudba.h"

char *sudba_make_string(char *text)
{		
	int text_length = strlen(text); 
  	char *temp = malloc(text_length+1);
	if (temp == NULL) { 
		return NULL; //"If malloc fails, return null"
	}
	int temp_counter = 0;
	for (int i = 1; i < text_length-1; i++) {
		if (text[i] == '\\') {
			switch(text[i+1]){
				case '\\': 
					temp[temp_counter] = '\\';
					break;
				case 'n':
					temp[temp_counter] = '\n';
					break;
				case 't':
					temp[temp_counter] = '\t';
					break;
				case 'r':
					temp[temp_counter] = '\r';
					break;
				case '"':
					temp[temp_counter] = '"';
					break;
				default:
					temp[temp_counter] = text[i+1];
					break;
			}
				i++;
				temp_counter++;
		}
		else {
			temp[temp_counter] = text[i];
			temp_counter++;
		}
	}
	temp[temp_counter] = '\0';
	temp = realloc(temp,temp_counter); 
  return temp; 
}

bool sudba_exists(char *table) {

	FILE *fptr;
	char frm[strlen(table)+strlen(DB_SCHEMA_EXT)+1];
	strcpy(frm,table); strcat(frm,DB_SCHEMA_EXT);
	// DZ: strlen(DB_WD) - why? You do not use it
	char MYD[strlen(DB_WD)+strlen(table)+strlen(DB_DATA_EXT)+1];
	strcpy(MYD,table); strcat(MYD,DB_DATA_EXT);

	if ((fptr = fopen(frm,"r")) == NULL) {
		return false;
	}
		fclose(fptr);
	if ((fptr = fopen(MYD,"r")) == NULL) {
		return false;
	}
		fclose(fptr);
	if ((fptr = fopen(frm,"a")) == NULL) {
		return false;
	}
		fclose(fptr);
	if ((fptr = fopen(MYD,"a")) == NULL) {
		return false;
	}
		fclose(fptr);
	
  /* 2 */
  // Your code goes here
  return true;
}

void sudba_lock(char *table) {
  // Do not modify
}

void sudba_unlock(char *table) {
  // Do not modify
}


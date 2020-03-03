#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sudba.h"
//"heyquote\"new tab\tnewline\nnewr\r"hey //DEBUGGING. WILL DELETE
//"heyquote\"new tab\tnew line\nrnewr\r51521" //DEBUGGING. WILL DELETE
//"heyquote\"new tab\tnew line\nnewr\r51521"  //DEBUGGING. WILL DELETE
//"quote\"newline\nslash\\doubleslash\\\\randomtab\trandomk\kdone!"yes  //DEBUGGING. WILL DELETE


char *sudba_make_string(char *text)
{		
	int text_length = strlen(text); 
  	char *temp = malloc(text_length+1);
	int temp_counter = 0;
	for (int i = 1; i < text_length-1; i++) {
		printf("text[%i] = %c\n",i,text[i]);
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
	printf("----------------------------------------------\n"); //DEBUGGING. WILL DELETE
	for (int j = 0; j < temp_counter+1; j++) {  //DEBUGGING. WILL DELETE
		printf("temp[%i] = %c\n",j,temp[j]);  //DEBUGGING. WILL DELETE
	}
	printf("temp_counter = %i\nFinal string: %s\n",temp_counter,temp);  //DEBUGGING. WILL DELETE
	temp = realloc(temp,temp_counter); 
  return temp; //was originally text
}

bool sudba_exists(char *table) {

	FILE *fptr;
	char frm[strlen(table)+strlen(DB_SCHEMA_EXT)+1];
	strcpy(frm,table); strcat(frm,DB_SCHEMA_EXT); 
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


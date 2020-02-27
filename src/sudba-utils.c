#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sudba.h"
//"heyquote\"new tab\tnewline\nnewr\r"hey
//"heyquote\"new tab\tnew line\nrnewr\r51521"
//"heyquote\"new tab\tnew line\nnewr\r51521"

char *sudba_make_string(char *text)
{		
	int text_length = strlen(text); 
  	char *temp = malloc(text_length+1);
	int temp_counter = 0;
	for (int i = 1; i < text_length-1; i++) {
		if (text[i] == '\\') {
			switch(text[i+1]){
				case '"': temp[temp_counter] = text[i+1];break;
			//if(text[i+1] == '"') {temp[temp_counter] = text[i+1];}
				case 't': temp[temp_counter] = '\t';break;
			//if(text[i+1] == 't') {temp[temp_counter] = '\t';}
				case 'n': temp[temp_counter] = '\n';break;
			//if(text[i+1] == 'n') {temp[temp_counter] = '\n';}
				case 'r': temp[temp_counter] = '\r';break;
			//if(text[i+1] == 'r') {temp[temp_counter] = '\r'; //ask dz about r
			//}
				case '\\': temp[temp_counter] = '\\';break;
			//if(text[i+1] == '\\') {temp[temp_counter] = '\\';} //remaining backslash??
			//text_length--; //for realloc
			}	
			temp_counter++;
			i++;
		}
		else {
			temp[temp_counter] = text[i];
			temp_counter++;
		}
		printf("text[%i] = %c\n", i,text[i]); //for debugging. will delete.
	}
	temp[temp_counter+1] = '\0';
	printf("temp counter: %i\n",temp_counter);
	temp = realloc(temp,temp_counter+1); //this code is buggy
	printf("text to return: %s\n",temp);
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


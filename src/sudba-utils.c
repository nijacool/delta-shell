#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sudba.h"
//"heyquote\"new tab\tnewline\nnewr\r"hey
//"heyquote\"new tab\tnew line\nrnewr\r51521"
//"heyquote\"new tab\tnew line\nnewr\r51521"

char *sudba_make_string(char *text)
{	
//<<<<<<< HEAD
	
	int text_length = strlen(text); // Do i need to catch this?
//????>>>>>>> b0b401328062147eca95a5ad816a687b19727711
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
		printf("text[%i] = %c\n", i,text[i]);
	}
	temp[text_length] = '\0';
	//temp = realloc(temp,text_length);
	puts(temp);
	//for (int j = 0; j < text_length; j++){
	//	printf("temp[%i] = %c\n", j,temp[j]);
	//}
  return text; //was originally text
}

bool sudba_exists(char *table) {
  printf("Table name:"); //need to delete
  puts(table);
	FILE *fptr;
	char frm[strlen(table)+strlen(DB_SCHEMA_EXT)+1];
	strcpy(frm,table); strcat(frm,DB_SCHEMA_EXT); 
	char MYD[strlen(DB_WD)+strlen(table)+strlen(DB_DATA_EXT)+1];
	strcpy(MYD,table); strcat(MYD,DB_DATA_EXT);
	printf("FRM: %s\n",frm);
	printf("MYD: %s\n",MYD);//will delete
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sudba.h"
//"heyquote\"new tab\tnewline\nnewr\r"hey
//"heyquote\"new tab\tnew line\nrnewr\r51521"
//"heyquote\"new tab\tnew line\nnewr\r51521"

char *sudba_make_string(char *text)
{	
	
	int text_length = strlen(text); // Do i need to catch this?
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
  printf("Table name:");
  puts(table);
	FILE *fptr;
	char frm[strlen(DB_WD)+strlen(table)+strlen(".frm")];
	strcpy(frm,DB_WD); strcat(frm,table); strcat(frm,".frm");
	printf("FRM directory name: %s\n",frm); //FOR DEBUGGING
	char MYD[strlen(DB_WD)+strlen(table)+strlen(".MYD")];
	strcpy(MYD,DB_WD); strcat(MYD,table); strcat(MYD,".MYD");
	printf("MYD directory name: %s\n",MYD); //FOR DEBUGGING
	if (((fptr = fopen(frm,"a+")) == NULL) || ((fptr = fopen(MYD,"a+")) == NULL)) { //I THINK THIS WORKS. JUST NEED TO FIND OUT HOW TO DEBUG //Also, ask DZ if it is okay to use just 1 fptr, since we are just checking, i don't think its a problem, but worth asking. & do we need to close fptr?
		return false;
	}

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


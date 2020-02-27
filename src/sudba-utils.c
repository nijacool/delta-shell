#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sudba.h"
//"heyquote\"new tab\tnewline\nnewr\r"hey
//"heyquote\"new tab\tnew line\nrnewr\r51521"
//"heyquote\"new tab\tnew line\nnewr\r51521"

char *sudba_make_string(char *text)
{	
	int text_length = strlen(text); // Do i need to catch this? no
  	char *temp = malloc(text_length+1);
	int temp_counter = 0;
	for (int i = 1; i < text_length-1; i++) {
		if (text[i] == '\\') {
//New Changes made from this point
			switch(text[i+1]){
				case '"': temp[temp_counter] = text[i+1];
					   break;
				case 't': temp[temp_counter] = '\t';
					   break;
				case 'r': temp[temp_counter] = '\r';
					   break;
				case '\\':temp[temp_counter] = '\\';
					   break;
			}
		
		}else{temp[temp_counter] = text[i];temp_counter++;}

//Code Taken Care as a switch statement, but kept the if statement version until clean up stage of the homework, to compare and check for errors

/*		
			if(text[i+1] == '"') {temp[temp_counter] = text[i+1];}
			if(text[i+1] == 't') {temp[temp_counter] = '\t';}
			if(text[i+1] == 'n') {temp[temp_counter] = '\n';}
			if(text[i+1] == 'r') {temp[temp_counter] = '\r'; //ask dz about r
		}
			if(text[i+1] == '\\') {temp[temp_counter] = '\\';
		} //remaining backslash??
			//text_length--; //for realloc
		}
		else {temp[temp_counter] = text[i];temp_counter++;}
*/


		printf("text[%i] = %c\n", i,text[i]);
	}

	temp[temp_counter] = '\0';
	temp = realloc(temp,temp_counter);
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
	char frm[strlen(DB_WD)+strlen(table)+strlen(".frm")];
	strcpy(frm,DB_WD); strcat(frm,table); strcat(frm,".frm"); //change this like in sudba-db.c
	printf("FRM directory name: %s\n",frm); //FOR DEBUGGING
	char MYD[strlen(DB_WD)+strlen(table)+strlen(".MYD")];
	strcpy(MYD,DB_WD); strcat(MYD,table); strcat(MYD,".MYD");
	printf("MYD directory name: %s\n",MYD); //FOR DEBUGGING
	if (((fptr = fopen(frm,"a+")) == NULL) || ((fptr = fopen(MYD,"a+")) == NULL)) { //I THINK THIS WORKS. JUST NEED TO FIND OUT HOW TO DEBUG //Also, ask DZ if it is okay to use just 1 fptr, since we are just checking, i don't think its a problem, but worth asking. & do we need to close fptr?
//need to check for reading first, then appending //4 opens= 4 closes

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


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
			if(text[i+1] == '"') {
				temp[temp_counter] = text[i+1];
		}
			if(text[i+1] == 't') {
				temp[temp_counter] = '\t';
		}
			if(text[i+1] == 'n') {
				temp[temp_counter] = '\n';
		}
			if(text[i+1] == 'r') {
				temp[temp_counter] = '\r'; //ask dz about r
		}
			if(text[i+1] == '\\') {
				temp[temp_counter] = '\\';
		} //remaining backslash??
			//text_length--; //for realloc
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
  /* 1 */
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


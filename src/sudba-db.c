#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudba.h"

bool sudba_drop_database(char *table) {
  sudba_lock(table);

	char frm[strlen(DB_WD)+strlen(table)+strlen(".frm")];
	strcpy(frm,DB_WD); strcat(frm,table); strcat(frm,".frm");
	char MYD[strlen(DB_WD)+strlen(table)+strlen(".MYD")];
	strcpy(MYD,DB_WD); strcat(MYD,table); strcat(MYD,".MYD");
	int status_frm; 
	int status_MYD;
  bool status = true;
  if (sudba_exists(table) == false) {
    fprintf(stdout, HTTP_VER " 404 Not Found %s\n\r", table);
    status = false;
  } else {  
    // Your code goes here
	status_frm = unlink(frm);
	status_MYD = unlink(MYD);
	printf("status_frm = %i, status_MYD = %i\n",status_frm,status_MYD); //FOR DEBUGGING WILL DELETE.
	if ((status_frm != 0) || (status_MYD != 0)) {
		fprintf(stdout, HTTP_VER "400 Error %s\n\r", table); //ASK DZ about this. HTTP VER???? should it  be there? 
	}
    fprintf(stdout, HTTP_VER " 200 Deleted %s\n\r", table);
  }
  
  sudba_unlock(table);
  free(table);
  return status ;
}

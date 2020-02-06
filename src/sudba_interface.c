#include <stdlib.h>
#include "sudba.h"
#include <unistd.h> //added by Sop Vong
#include <string.h> //added
#include <stdio.h>
#include <errno.h> //added by Cristian 
#include <syslog.h> //included by Cristian

// Client-side SUDBa interface

char *sudba_read_line(int file)
{	
	char buf[SUDBA_MAX_INPUT]; //Creates a static buffer called buf with size SUDBA_MAX_INPUT (defined in sudba.h) 
	int count = read(file, buf, SUDBA_MAX_INPUT); //Reads and stores the number of characters in buf, up to SUDBA_MAX_INPUT

	if (count == -1) {
		strerror_r(errno, buf, SUDBA_MAX_INPUT); //added by Cristian To check if read as an error
		syslog(LOG_ERR,"%s", buf);
		return NULL;
		
	}


	//strerror_r(file, buf, SUDBA_MAX_INPUT); //file might not be the correct argument !!
	printf("Count: %d\n", count); // testing purposes
	char *r = malloc(count + 1); //Allocates dynamic memory r with size of the input +1 for the NULL-terminated character

	if (r == NULL) {
		strerror_r(errno, buf, SUDBA_MAX_INPUT); //added by Cristian To check if malloc as an error
		syslog(LOG_ERR,"%s", buf);
		return NULL;
	}
	


	//strerror_r(file, buf, SUDBA_MAX_INPUT); //file might not be the correct argument !!
 	int i = 0;
	//We need a for loop because we know the number of iterations, while loop is only use if we do not know the number of iterations;

	for(i = 0; i < count; i++){
		r[i] = buf[i];
	}

	r[i] = 0; //RESOLVED! Ask Dmitry is index i+1 supposed to be there????????????????????????????????

	int done = 0;
	
	while (read(file, buf, SUDBA_MAX_INPUT) > 0) {  //while you read at least one character, it will keep reading in an iteration, to discard any exceesive input.
	} 


	//printf("Final int i is %d\n", i); //testing purposes
	
	puts(r); //testing purposes
	for (int c = 0; c < count; c = c+1){ //this is for testing purposes to print each index
	printf("r[%d] : %c \n",c,r[c]); //this is for testing purposes to print each index
	}
	syslog(LOG_INFO, "%s", r); // The function shall log the obtained string to the previously openend system log file at priority LOG_INFO before returning it. use the command tail /var/log/syslog in the terminal to read the log that are save in the log file
	return r; // Must be changed
}

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
	char static_buffer[SUDBA_MAX_INPUT]; //Creates a static buffer called static_buffer with size SUDBA_MAX_INPUT (defined in sudba.h) 
	int buffer_read_size = read(file, static_buffer, SUDBA_MAX_INPUT); //Reads and stores the number of characters in static_buffer, up to SUDBA_MAX_INPUT

	if (buffer_read_size == -1) {
		strerror_r(errno, static_buffer, SUDBA_MAX_INPUT); //Added by Cristian To check if read as an error
		syslog(LOG_ERR,"%s", static_buffer);
		return NULL;
	}

	//printf("buffer_read_size: %d\n", buffer_read_size); //For testing purposes to print the number of characters read.
	char *string_pointer = malloc(buffer_read_size + 1); //Allocates dynamic memory string_pointer with size of the input. An additional character added (buffer_read_size+1) for null character 0

	if (string_pointer == NULL) {
		strerror_r(errno, static_buffer, SUDBA_MAX_INPUT); //added by Cristian To check if malloc as an error
		syslog(LOG_ERR,"%s", static_buffer);
		return NULL;
	}

 	int i = 0; //We need a for loop because we know the number of iterations, while loop is only use if we do not know the number of iterations;

	for(i = 0; i < buffer_read_size; i++){
		string_pointer[i] = static_buffer[i];
	}

	string_pointer[i] = 0; //RESOLVED! Ask DZ is index i+1 supposed to be there?

	while (read(file, static_buffer, SUDBA_MAX_INPUT) > 0) {  //While you read at least one character, it will keep reading in an iteration, to discard any exceesive input.
	} 

	//puts(string_pointer); //For testing purposes

	/*for (int c = 0; c < buffer_read_size; c = c+1) { //this is for testing purposes to print each index
	printf("string_pointer[%d] : %c \n", c, string_pointer[c]); // For testing purposes to print each index char[i]
	}*/

	syslog(LOG_INFO, "%s", string_pointer); // The function shall log the obtained string to the previously openend system log file at priority LOG_INFO before returning it. use the command tail /var/log/syslog in the terminal to read the log that are save in the log file
	return string_pointer; 
}

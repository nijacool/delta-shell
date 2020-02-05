#include <stdlib.h>
#include "sudba.h"
#include <unistd.h> //added by Sop Vong
#include <string.h> //added
#include <stdio.h>

// Client-side SUDBa interface

char *sudba_read_line(int file)
{	
	char buf[SUDBA_MAX_INPUT]; //Creates a static buffer called buf with size SUDBA_MAX_INPUT (defined in sudba.h) 
	int count = read(file, buf, SUDBA_MAX_INPUT); //Reads and stores the number of characters in buf, up to SUDBA_MAX_INPUT
	//strerror_r(file, buf, SUDBA_MAX_INPUT); //file might not be the correct argument !!
	printf("Count: %d\n", count); // testing purposes
	char *r = malloc(count + 1); //Allocates dynamic memory r with size of the input +1 for the NULL-terminated character
	//strerror_r(file, buf, SUDBA_MAX_INPUT); //file might not be the correct argument !!
 	int i = 0;
	while (i < count) { //This while loop copies the iTh character from the static buffer buf[SUDBA_MAX_INPUT] and stores it in the dynamic buffer r
		r[i] = buf[i];
		i = i + 1;
	}
	r[i] = 0; //RESOLVED! Ask Dmitry is index i+1 supposed to be there????????????????????????????????

	int done = 0;
	while (done == 0) {
		if (read(file, buf, SUDBA_MAX_INPUT) == 0) done = 1;
	} 


	//printf("Final int i is %d\n", i); //testing purposes
	
	puts(r); //testing purposes
	for (int c = 0; c < count; c = c+1){ //this is for testing purposes to print each index
	printf("r[%d] : %c \n",c,r[c]); //this is for testing purposes to print each index
	}
	return r; // Must be changed
}

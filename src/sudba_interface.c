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
	printf("Count: %d\n", count); // testing purposes
	char *r = malloc(count + 1); //Allocates dynamic memory r with size of the input +1 for the NULL-terminated character
 


	//int done = read(file, buf, SUDBA_MAX_INPUT);
	//printf("done = %d", done); //Ask Dmitry how to use read in a while loop, so that it isn't asking for another input, basically how to read again and pick up from where it left the previous one. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//done = read(file, buf, SUDBA_MAX_INPUT);
	//printf("new done = %d", done);
	/*while (done != 0) {
		if (read(file, buf, SUDBA_MAX_INPUT) == 0){
			done = 0;	
		}
		printf("Read isn't 0");
	}*/	



	int i = 0;
	while (i < count) { //This while loop copies the iTh character from the static buffer buf[SUDBA_MAX_INPUT] and stores it in the dynamic buffer r
		r[i] = buf[i];
		i = i + 1;
	}
	printf("Final int i is %d\n", i); //testing purposes
	r[i-1] = '0'; //Ask Dmitry is index i+1 supposed to be there? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int len = strlen(r); //this is for testing purposes to print each index
	for (int c = 0; c <= len; c = c+1){ //this is for testing purposes to print each index
	printf("r[%d] : %c \n",c,r[c]); //this is for testing purposes to print each index
	}


	//free(*buf); // Ask Dmitry, we can't free buf, but when we free r then it don't work in sudba.c !!!!!!!!!!!!!!!!
	

  return r; // Must be changed
}

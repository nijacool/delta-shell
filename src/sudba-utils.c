#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sudba.h"

char *sudba_make_string(char *text) {
  int text_length = strlen(text);
  char *temp = malloc(text_length + 1);
  if (!temp)
    return NULL;

  int j = 0;
  for (int i = 1; /* skip the first " */
       i < text_length - 1; /* skip the last " */
       i++, j++) {
    if (text[i] == '\\') { /* if an escape character */
      i++;
      switch(text[i]) {
      case '\\':
	temp[j] = '\\';
	break;
      case 'n':
	temp[j] = '\n';
	break;
      case 't':
	temp[j] = '\t';
	break;
      case 'r':
	temp[j] = '\r';
	break;
      case '"':
	temp[j] = '"';
	break;
      default:
	temp[j] = text[i];
	break;
      }
    } else
      temp[j] = text[i];
  }
  temp[j] = '\0'; /* NULL-terminate */
  temp = realloc(temp, j + 1); /* Trim as needed */
  return text;
}

bool sudba_exists(char *table) {
  char schema[strlen(table) + sizeof(DB_SCHEMA_EXT)];
  char data  [strlen(table) + sizeof(DB_DATA_EXT  )];
  sprintf(schema, "%s" DB_SCHEMA_EXT, table);
  sprintf(data  , "%s" DB_DATA_EXT  , table);

  FILE *fptr;
  /* Are both table files readable and writable? */
  return (fptr = fopen(schema, "r")) && !fclose(fptr)
      && (fptr = fopen(schema, "a")) && !fclose(fptr)
      && (fptr = fopen(data,   "r")) && !fclose(fptr)
      && (fptr = fopen(data,   "a")) && !fclose(fptr);
}

void sudba_lock(char *table) {
  // Do not modify
}

void sudba_unlock(char *table) {
  // Do not modify
}

void *my_malloc(size_t size) {
	char *s;
	s = malloc(size);
	if (s == NULL) { abort(); }
  return s;
}

void *my_realloc(void *ptr, size_t size) {
	char *s;
	s = realloc(ptr, size);
  if (s == NULL) { abort (); }
  return s;
}

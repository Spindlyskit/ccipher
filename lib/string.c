#include "libccipher/string.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void str_upper(char *string)
{
	str_foreach(string) {
		*string = toupper(*string);
	}
}

void str_lower(char *string)
{
	str_foreach(string) {
		*string = tolower(*string);
	}
}

char *str_clean(char *string)
{
	// Create the new string and a running total of its length
	char *new = malloc(strlen(string) + 1);
	int nl = 0;

	// Push alphabetic characters to the new string, replacing all uppercase characters with a "u" and all lowercase
	// with an "l", leaves other characters untouched without adding them to the new string
	str_foreach(string) {
		if (isupper(*string)) {
			new[nl] = *string;
			*string = 'u';
			nl++;
		} else if (islower(*string)) {
			new[nl] = toupper(*string);
			*string = 'l';
			nl++;
		} else {
			*string = *string;
		}
	}

	new[nl] = '\0';

	return new;
}

void str_repair(char *schema, char *string)
{
	int len = strlen(string);
	int c = 0;

	str_foreach(schema) {
		// If the counter is out of range, we have processed all necessary characters
		if (c >= len) {
			break;
		}

		if (*schema == 'u') {
			*schema = toupper(string[c]);
			c++;
		} else if (*schema == 'l') {
			*schema = tolower(string[c]);
			c++;
		}
		// Don't transform non alpha characters
	}
}

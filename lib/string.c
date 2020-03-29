#include "libccipher/string.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char ALPHABET[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char ALPHABET_STRING[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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

void str_shuffle(char *string)
{
	int len = strlen(string);
	char swap;

	for (int i = 0; i < len; i++) {
		int random = rand() % len;
		swap = string[random];
		string[random] = string[i];
		string[i] = swap;
	}
}

void str_swap(char *string, size_t c1, size_t c2)
{
	// Don't do anything if the indices are the same
	if (c1 == c2) {
		return;
	}

	char swap = string[c1];
	string[c1] = string[c2];
	string[c2] = swap;
}

char *str_clean(char *string)
{
	// Create the new string and a running total of its length
	char *new = malloc(strlen(string) + 1);
	if (!new) {
		return NULL;
	}
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

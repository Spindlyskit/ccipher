#ifndef CCIPHER_STRING_H
#define CCIPHER_STRING_H

#include <stddef.h>

// Loop over each character of a string
#define str_foreach(x) for (; *(x) != '\0'; (x)++)

// The uppercase alphabet
extern const char ALPHABET[26];

// Convert a string to uppercase in place
void str_upper(char *string);

// Convert a string to lowercase in place
void str_lower(char *string);

// Shuffle a string
void str_shuffle(char *string);

// Swap to characters in a string
void str_swap(char *string, size_t c1, size_t c2);

// Remove non-alpha characters and uppercase a string, the string becomes a repair schema in place
char *str_clean(char *string);

// Repair a string using a schema, this schema transforms into the new string in place
void str_repair(char *schema, char *string);

#endif //CCIPHER_STRING_H

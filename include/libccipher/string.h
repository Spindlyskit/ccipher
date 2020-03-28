#ifndef CCIPHER_STRING_H
#define CCIPHER_STRING_H

// Loop over each character of a string
#define str_foreach(x) for (; *x != '\0'; x++)

// Convert a string to uppercase in place
void str_upper(char *string);

// Convert a string to lowercase in place
void str_lower(char *string);

// Remove non-alpha characters and uppercase a string, the string becomes a repair schema in place
char *str_clean(char *string);

// Repair a string using a schema, this schema transforms into the new string in place
void str_repair(char *schema, char *string);

#endif //CCIPHER_STRING_H

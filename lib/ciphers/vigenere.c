#include <string.h>

void vigenere_solve(char *key, const char *text, char *dest)
{
	int i;
	int key_length = strlen(key);

	for (i = 0; text[i] != '\0'; i++) {
		dest[i] = (text[i] + 26 - key[i % key_length]) % 26 + 'A';
	}

	dest[i] = '\0';
}

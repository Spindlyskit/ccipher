#include "libccipher/ciphers/substitution.h"

void substitution_solve(char key[26], char *text)
{
	for (char *x = text; *x != '\0'; x++) {
		if (*x < 'A' || *x > 'Z') {
			continue;
		}

		*x = key[(int) (*x - 'A')];
	}
}

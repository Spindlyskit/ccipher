#include "libccipher/scorer.h"

void caesar_solve(int key, char *text)
{
	for (int i = 0; text[i] != '\0'; i++) {
		char *c = text + i;

		if (*c < 'A' || *c > 'Z') {
			continue;
		}
		*c += key;

		if (*c > 'Z') {
			*c -= 26;
		}
	}
}

char *caesar_crack(struct text_scorer, char *text);

#include "libccipher/ciphers/caesar.h"

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "libccipher/scorer.h"
#include "libccipher/string.h"

bool caesar_parse_key(const char *text, unsigned int *key)
{
	char *c = text;

	str_foreach(c) {
		if (!isdigit(*c)) {
			return false;
		}
	}

	*key = strtoul(text, NULL, 10);

	if (*key > 26) {
		return false;
	}

	return true;
}

void caesar_solve(unsigned int key, char *text, char *dest)
{
	int i;
	for (i = 0; text[i] != '\0'; i++) {
		dest[i] = text[i] + key;

		if (dest[i] > 'Z') {
			dest[i] -= 26;
		}
	}

	dest[i] = '\0';
}

unsigned int caesar_crack(struct text_scorer *scorer, char *text, char *dest)
{
	unsigned int best_key = 0;
	float best_score = scorer_quadgram_score(scorer, text);

	// Copy the cipher text into the destination
	strcpy(dest, text);

	for (unsigned int key = 1; key < 26; key++) {
		// Shift the text by one
		caesar_solve(1, dest, dest);

		float score = scorer_quadgram_score(scorer, dest);

		if (score > best_score) {
			best_key = key;
			best_score = score;
		}
	}

	caesar_solve(best_key, text, dest);
	return best_key;
}

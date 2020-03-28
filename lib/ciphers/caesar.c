#include "libccipher/ciphers/caesar.h"

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "libccipher/scorer.h"
#include "libccipher/string.h"

bool caesar_parse_key(char *text, unsigned int *key)
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

void caesar_solve(unsigned int key, char *c)
{
	str_foreach(c) {
		if (!isalpha(*c)) {
			continue;
		}
		*c += key;

		if (*c > 'Z') {
			*c -= 26;
		}
	}
}

unsigned int caesar_crack(struct text_scorer *scorer, char *text)
{
	unsigned int best_key;
	float best_score = -INFINITY;

	for (unsigned int i = 1; i <= 26; i++) {
		caesar_solve(1, text);
		float score = scorer_quadgram_score(scorer, text);

		if (score > best_score) {
			best_key = i;
			best_score = score;
		}
	}

	caesar_solve(best_key, text);
	return best_key;
}

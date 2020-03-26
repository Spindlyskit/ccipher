#include "libccipher/ciphers/caesar.h"

#include <math.h>

#include "libccipher/scorer.h"

void caesar_solve(unsigned int key, char *text)
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

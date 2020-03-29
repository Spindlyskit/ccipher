#include "libccipher/ciphers/caesar.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "libccipher/scorer.h"
#include "libccipher/string.h"

void caesar_with_data(struct cipher_data *data)
{
	if (data->use_autocrack) {
		unsigned int used_key = caesar_crack(data->scorer, data->ct, data->result);
		data->success = true;
		sprintf(data->key, "%u", used_key);
		return;
	}

	unsigned int key;
	if (!caesar_parse_key(data->key, &key)) {
		return;
	}

	caesar_solve(key, data->ct, data->result);
	data->success = true;
}

bool caesar_parse_key(const char *text, unsigned int *key)
{
	const char *c = text;

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

void caesar_solve(unsigned int key, const char *text, char *dest)
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

unsigned int caesar_crack(const struct text_scorer *scorer, const char *text, char *dest)
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

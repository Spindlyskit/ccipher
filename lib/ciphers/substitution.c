#include "libccipher/ciphers/substitution.h"

#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "libccipher/ccipher.h"
#include "libccipher/logging.h"
#include "libccipher/scorer.h"
#include "libccipher/string.h"

#define MAX_ITERATIONS 5
#define INTERNAL_ITERATIONS 1000

static void crack_single(const struct text_scorer *scorer, const char *ct, char best_key[27], float *best_score);

// Crack a substitution cipher once - should be run multiple times to avoid local maximums
static void crack_single(const struct text_scorer *scorer, const char *ct, char best_key[27], float *best_score)
{
	strcpy(best_key, ALPHABET);
	str_shuffle(best_key);

	log_debug("Substitution generated initial key %s", best_key);

	// Allocate a new string to store the decoded values
	char *decoded = malloc(strlen(ct) + 1);
	if (!decoded) {
		log_error("Failed to allocate memory for decoded text");
	}

	strcpy(decoded, ct);
	substitution_solve(best_key, ct, decoded);
	*best_score = scorer_quadgram_score(scorer, decoded);
	float current_score;

	char current_key[27];
	strcpy(current_key, best_key);

	for (int i = 0; i < INTERNAL_ITERATIONS; i++) {
		// Swap a random two characters
		str_swap(current_key, rand() % 26, rand() % 26);

		substitution_solve(current_key, ct, decoded);
		current_score = scorer_quadgram_score(scorer, decoded);

		if (current_score > *best_score) {
			// Reset the counter and replace best score
			i = 0;
			*best_score = current_score;
			strcpy(best_key, current_key);
		} else {
			strcpy(current_key, best_key);
		}
	}

	free(decoded);
}

void substitution_with_data(struct cipher_data *data)
{
	if (data->use_autocrack) {
		 char *used_key = substitution_crack(data->scorer, data->ct, data->result);
		data->success = true;
		sprintf(data->key, "%s", used_key);
		return;
	}

	char key[27];
	if (!substitution_parse_key(data->key, key)) {
		return;
	}

	substitution_solve(key, data->ct, data->result);
	data->success = true;
}

bool substitution_parse_key(const char *text, char key[27])
{
	unsigned int i;

	// Check the length is 26, and each letter appears once
	for (i = 0; i < 26; i++) {
		// Check the character is a letter
		// This will also fail if the character is '\0' so we won't segfault
		if (!isalpha(text[i])) {
			return false;
		}

		key[i] = toupper(text[i]);
		char c = key[i];

		// Check for duplicates
		for (unsigned int j = 0; j < i; j++) {
			if (key[j] == c) {
				return false;
			}
		}
	}

	key[26] = '\0';

	// Ensure the string is only 26 long
	return text[i] == '\0';
}

void substitution_solve(const char key[27], const char *text, char *dest)
{
	unsigned int i;
	for (i = 0; text[i] != '\0'; i++) {
		dest[i] = key[text[i] - 'A'];
	}

	dest[i] = '\0';
}

char *substitution_crack(const struct text_scorer *scorer, const char *text, char *dest)
{
	char *best_key = malloc(27);
	float best_score = -INFINITY;

	for (int i = 0; i < MAX_ITERATIONS; i++) {
		char current_key[27];
		float current_score;

		crack_single(scorer, text, current_key, &current_score);

		if (current_score > best_score) {
			best_score = current_score;
			strcpy(best_key, current_key);
		}
	}

	substitution_solve(best_key, text, dest);
	return best_key;
}

#include "libccipher/ciphers/substitution.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "libccipher/logging.h"
#include "libccipher/scorer.h"
#include "libccipher/string.h"

#define MAX_ITERATIONS 5
#define INTERNAL_ITERATIONS 1000

static void crack_single(struct text_scorer *scorer, char *ct, char best_key[26], float *best_score);
static void substitution(char key[26], char *text, char *dest);

// Crack a substitution cipher once - should be run multiple times to avoid local maximums
static void crack_single(struct text_scorer *scorer, char *ct, char best_key[26], float *best_score)
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
	substitution(best_key, ct, decoded);
	*best_score = scorer_quadgram_score(scorer, decoded);
	float current_score;

	char current_key[26];
	strcpy(current_key, best_key);

	for (int i = 0; i < INTERNAL_ITERATIONS; i++) {
		// Swap a random two characters
		str_swap(current_key, rand() % 26, rand() % 26);

		substitution(current_key, ct, decoded);
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

// Perform a substitution, placing the result in dest
// It is assumed that dest has space for the result
static void substitution(char key[26], char *text, char *dest)
{
	for (int i = 0; text[i] != '\0'; i++) {
		dest[i] = key[(int) (text[i] - 'A')];
	}
}

void substitution_solve(char key[26], char *text)
{
	substitution(key, text, text);
}

char *substitution_crack(struct text_scorer *scorer, char *text)
{
	char *best_key = malloc(27);
	float best_score = -INFINITY;

	for (int i = 0; i < MAX_ITERATIONS; i++) {
		char current_key[26];
		float current_score;

		crack_single(scorer, text, current_key, &current_score);

		if (current_score > best_score) {
			best_score = current_score;
			strcpy(best_key, current_key);
		}
	}

	substitution(best_key, text, text);
	return best_key;
}

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "libccipher/scorer.h"

#define MAX_KEY_LENGTH 26

// Decode a vigenere cipher with a given key and key length
static void decode(const char *key, size_t key_length, const char *text, char *dest)
{
	int i;

	for (i = 0; text[i] != '\0'; i++) {
		dest[i] = (text[i] + 26 - key[i % key_length]) % 26 + 'A';
	}

	dest[i] = '\0';
}

void vigenere_solve(const char *key, const char *text, char *dest)
{
	decode(key, strlen(key), text, dest);
}

char *vigenere_crack(const struct text_scorer *scorer, const char *text, char *dest)
{
	float best_score = -INFINITY;
	int len = strlen(text);
	// We don't need to try keys that are longer than the text
	int max_key_len = MAX_KEY_LENGTH < len ? MAX_KEY_LENGTH : len;

	// Leave room for the null character at the end
	char key[MAX_KEY_LENGTH + 1];
	int best_key_len = max_key_len;
	char *best_key = malloc(sizeof(key));
	key[MAX_KEY_LENGTH] = '\0';

	// Go through all possible key lengths from 2-MAX_KEY_LENGTH
	for (int key_len = 2; key_len <= max_key_len; key_len++) {
		memset(key, 'A', key_len);

		// Keep track of the score of the best key we have found in this iteration
		float local_best_score = -INFINITY;

		for (int i = 0; i < key_len; i++) {
			char best_char = 'A';

			// Try each possible letter and find the best one
			for (; key[i] <= 'Z'; key[i]++) {
				decode(key, key_len, text, dest);
				float score = scorer_quadgram_score(scorer, dest);

				if (score > local_best_score) {
					local_best_score = score;
					best_char = key[i];
				}
			}

			key[i] = best_char;
		}

		if (local_best_score > best_score) {
			strcpy(best_key, key);
			best_key_len = key_len;
			best_score = local_best_score;
		}
	}

	decode(best_key, best_key_len, text, dest);
	best_key[best_key_len] = '\0';
	return best_key;
}

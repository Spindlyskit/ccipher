#ifndef CCIPHER_SCORER_H
#define CCIPHER_SCORER_H

#include <stdio.h>

#define QUADGRAM_LENGTH 456976

struct text_scorer {
	float quadgrams[QUADGRAM_LENGTH];
};

// Get the index of an ngram of n length in a sorted array
// The behaviour is undefined if the length of ngram < n, or the characters aren't {A..Z}
int get_ngram_index(const char *ngram, int length);

// Load quadgram data from a given file
void scorer_load_data(struct text_scorer *scorer, FILE *quadgram_file);

// Score text based on quadgrams
float scorer_quadgram_score(struct text_scorer *scorer, const char *text);

#endif //CCIPHER_SCORER_H

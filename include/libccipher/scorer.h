#ifndef CCIPHER_SCORER_H
#define CCIPHER_SCORER_H

#include <stdio.h>

#define QUADGRAM_LENGTH 456976

struct text_scorer {
	float quadgrams[QUADGRAM_LENGTH];
};

// Load quadgram data from a given file
void scorer_load_data(struct text_scorer *scorer, FILE *quadgram_file);

// Score text based on quadgrams
float scorer_quadgram_score(struct text_scorer *scorer, char *text);

#endif //CCIPHER_SCORER_H

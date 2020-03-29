#include "libccipher/scorer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libccipher/logging.h"

int get_ngram_index(const char *ngram, int length)
{
	int idx = 0, digit = 1;

	for (int i = length - 1; i >= 0; i--) {
		idx += (ngram[i] - 'A') * digit;
		digit *= 26;
	}

	return idx;
}

void scorer_load_data(struct text_scorer *scorer, FILE *quadgram_file)
{
	if (quadgram_file == NULL) {
		log_fatal("Failed to read quadgram file");
		exit(EXIT_FAILURE);
	}

	char line[256];
	for (int i = 0; fgets(line, sizeof(line), quadgram_file); i++) {
		if (i > QUADGRAM_LENGTH) {
			log_error("Quadgram file too long, skipping excess quadgrams");
			break;
		}

		scorer->quadgrams[i] = strtof(line, NULL);
		if (scorer->quadgrams[i] == 0.0) {
			log_error("Failed to load quadgram with probability %s", line);
		}
	}
}

float scorer_quadgram_score(const struct text_scorer *scorer, const char *text)
{
	unsigned long len = strlen(text);
	if (len < 4) {
		log_error("Cannot quadgram score text \"%s\" of length %d", text, len);
		return 0.0f;
	}

	float total = 0.0f;
	for (int i = 0; i < len - 3; i++) {
		total += scorer->quadgrams[get_ngram_index(text + i, 4)];
	}

	return total;
}

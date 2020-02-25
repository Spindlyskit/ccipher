#include "libccipher/scorer.h"

#include <stdlib.h>
#include <stdio.h>

#include "libccipher/logging.h"

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

float scorer_quadgram_score(struct text_scorer *scorer, char *text)
{
	return 0;
}

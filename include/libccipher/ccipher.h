#ifndef CCIPHER_CCIPHER_H
#define CCIPHER_CCIPHER_H

#include <stdbool.h>

#include "libccipher/scorer.h"

// Represents the inputs and outputs for a cipher
// Scorer can be omitted if use_autocrack is true
// The used key will be stored in key if autocrack is used
// Behaviour is undefined if ct is not cleaned (only uppercase alphabetic characters)
struct cipher_data {
	const char *ct;
	char *key;
	bool use_autocrack;
	const struct text_scorer *scorer;
	char *result;
	bool success;
};

#endif //CCIPHER_CCIPHER_H

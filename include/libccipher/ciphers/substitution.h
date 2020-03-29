#ifndef CCIPHER_SUBSTITUTION_H
#define CCIPHER_SUBSTITUTION_H

#include <stdbool.h>
#include "libccipher/ccipher.h"
#include "libccipher/scorer.h"

// Run a substitution cipher using a cipher_data
void substitution_with_data(struct cipher_data *data);

// Parse a substitution key from a string
bool substitution_parse_key(const char *text, char key[26]);

// Solve a substitution cipher with the given key
void substitution_solve(const char *key, const char *text, char *dest);

// Automatically crack a substitution cipher, returns the used key
// The returned value must be freed
char *substitution_crack(const struct text_scorer *scorer, const char *text, char *dest);

#endif //CCIPHER_SUBSTITUTION_H

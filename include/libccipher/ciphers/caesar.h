#ifndef CCIPHER_CAESAR_H
#define CCIPHER_CAESAR_H

#include <stdbool.h>

#include "libccipher/ccipher.h"
#include "libccipher/scorer.h"

// Run a caesar cipher using a cipher_data
void caesar_with_data(struct cipher_data *data);

// Parse a caesar key from a string
bool caesar_parse_key(const char *text, unsigned int *key);

// Solve a caesar cipher with the given key
void caesar_solve(unsigned int key, const char *text, char *dest);

// Automatically crack a caesar cipher, returns the used key
unsigned int caesar_crack(const struct text_scorer *scorer, const char *text, char *dest);

#endif //CCIPHER_CAESAR_H

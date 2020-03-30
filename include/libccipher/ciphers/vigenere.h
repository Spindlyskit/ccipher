#ifndef CCIPHER_VIGENERE_H
#define CCIPHER_VIGENERE_H

#include <stdbool.h>

#include "libccipher/ccipher.h"
#include "libccipher/scorer.h"

// Run a vigenere cipher using a cipher_data
void vigenere_with_data(struct cipher_data *data);

// Parse a vigenere key from a string
bool vigenere_parse_key(const char *text, char *key);

// Solve a vigenere cipher with the given key
void vigenere_solve(char *key, const char *text, char *dest);

// Automatically crack a vigenere cipher, returns the used key
char *vigenere_crack(const struct text_scorer *scorer, const char *text, char *dest);

#endif //CCIPHER_VIGENERE_H

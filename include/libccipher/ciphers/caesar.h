#ifndef CCIPHER_CAESAR_H
#define CCIPHER_CAESAR_H

#include <libccipher/scorer.h>

// Parse a caesar key from a string
unsigned int caesar_parse_key(char *text);

// Solve a caesar cipher with the given key
void caesar_solve(unsigned int key, char *text);

// Automatically crack a caesar cipher, returns the used key
unsigned int caesar_crack(struct text_scorer *scorer, char *text);

#endif //CCIPHER_CAESAR_H

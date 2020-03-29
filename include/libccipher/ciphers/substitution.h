#ifndef CCIPHER_SUBSTITUTION_H
#define CCIPHER_SUBSTITUTION_H

#include <stdbool.h>
#include <libccipher/scorer.h>

// Parse a substitution key from a string
bool substitution_parse_key(char *text, char key[26]);

// Solve a substitution cipher with the given key
void substitution_solve(char *key, char *text);

// Automatically crack a substitution cipher, returns the used key
// The returned value must be freed
char *substitution_crack(struct text_scorer *scorer, char *text);

#endif //CCIPHER_SUBSTITUTION_H

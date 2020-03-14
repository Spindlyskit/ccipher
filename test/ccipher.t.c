#include <unity.h>
#include <string.h>
#include <libccipher/scorer.h>
#include <libccipher/ciphers/caesar.h>

void setUp() {}
void tearDown() {}

void test_get_ngram_index(void)
{
	// Test monograms
	char *test_string = "SUBSTITUTE";
	TEST_ASSERT_EQUAL_INT(1, get_ngram_index(test_string + 2, 1));
	TEST_ASSERT_EQUAL_INT(19, get_ngram_index(test_string + 4, 1));

	// Test quadgrams
	TEST_ASSERT_EQUAL_INT(0, get_ngram_index("AAAA SOME OTHER TEXT", 4));
	TEST_ASSERT_EQUAL_INT(234257, get_ngram_index("NINXXXAUDHAIDF", 4));
	TEST_ASSERT_EQUAL_INT(339787, get_ngram_index("TIQT", 4));
	TEST_ASSERT_EQUAL_INT(456975, get_ngram_index("ZZZZ", 4));
}

void test_load_quadgrams(void)
{
	struct text_scorer scorer;
	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	TEST_ASSERT_EQUAL_FLOAT(-5.79933, scorer.quadgrams[0]);
	TEST_ASSERT_EQUAL_FLOAT(-7.34470, scorer.quadgrams[3167]);
	TEST_ASSERT_EQUAL_FLOAT(-6.78125, scorer.quadgrams[QUADGRAM_LENGTH - 1]);
}

void test_scorer_quadgram_score(void)
{
	// Load the test data
	struct text_scorer scorer;
	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	// Test scoring single quadgrams
	TEST_ASSERT_EQUAL_FLOAT(-4.034544, scorer_quadgram_score(&scorer, "APPL"));
	TEST_ASSERT_EQUAL_FLOAT(-5.179962, scorer_quadgram_score(&scorer, "AULS"));

	// Test scoring actual text
	char *string_1 = "WENEEDAWAYOFDETERMININGIFAPIECEOFTEXTISVERYSIMILARTOENGLISHTHISISACHIEVEDBYCOUNTINGQUADGRAMS";
	char *string_2 = "AASJHAFKJHDSJKGFUAKSHDJASHGBCHJXCGYXZFCYZXGVJKZXKBVGZJXHVGZJKXBCHJZBCJBDCHJZBCJXVBHJZXBJKZXV";
	TEST_ASSERT_EQUAL_FLOAT(-389.3947, scorer_quadgram_score(&scorer, string_1));
	TEST_ASSERT_EQUAL_FLOAT(-887.977, scorer_quadgram_score(&scorer, string_2));
}

void test_cipher_caesar_solve(void)
{
	char text[] = "AOPZ PZ H JHLZHY JPWOLY";
	caesar_solve(19, text);

	TEST_ASSERT_EQUAL_STRING("THIS IS A CAESAR CIPHER", text);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_get_ngram_index);
	RUN_TEST(test_load_quadgrams);
	RUN_TEST(test_scorer_quadgram_score);
	RUN_TEST(test_cipher_caesar_solve);
	return UNITY_END();
}
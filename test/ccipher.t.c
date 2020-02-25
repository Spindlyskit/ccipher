#include <unity.h>
#include <libccipher/scorer.h>

void setUp() {}
void tearDown() {}

void get_ngram_index_should_return_index(void)
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

void load_quadgrams_should_load_quadgrams(void)
{
	struct text_scorer scorer;

	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	// The expected values are more precise than floats, so they should be made shorter
	TEST_ASSERT_EQUAL_FLOAT(-5.799338278530059, scorer.quadgrams[0]);
	TEST_ASSERT_EQUAL_FLOAT(-7.34470369346995, scorer.quadgrams[3167]);
	TEST_ASSERT_EQUAL_FLOAT(-6.781259884971996, scorer.quadgrams[QUADGRAM_LENGTH - 1]);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(get_ngram_index_should_return_index);
	RUN_TEST(load_quadgrams_should_load_quadgrams);
	return UNITY_END();
}
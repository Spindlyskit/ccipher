#include <unity.h>
#include <libccipher/ccipher.h>
#include <libccipher/scorer.h>

void setUp() {}
void tearDown() {}

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
	RUN_TEST(load_quadgrams_should_load_quadgrams);
	return UNITY_END();
}
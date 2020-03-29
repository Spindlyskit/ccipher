#include <stdbool.h>
#include <stdlib.h>
#include <unity.h>
#include <string.h>
#include <libccipher/scorer.h>
#include <libccipher/logging.h>
#include <libccipher/string.h>
#include <libccipher/ciphers/caesar.h>
#include <libccipher/ciphers/substitution.h>

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

void test_string_case_change(void)
{
	char string[] = "Hello, World!";
	char string2[] = "GoodBye World.";

	str_upper(string);
	str_lower(string2);

	TEST_ASSERT_EQUAL_STRING("HELLO, WORLD!", string);
	TEST_ASSERT_EQUAL_STRING("goodbye world.", string2);
}

void test_string_schema(void)
{
	char string[] = "Hello, wOrld!";

	char *new = str_clean(string);
	if (!new) {
		TEST_FAIL_MESSAGE("Malloc failed for str_clean");
	}

	TEST_ASSERT_EQUAL_STRING("HELLOWORLD", new);
	TEST_ASSERT_EQUAL_STRING("ullll, lulll!", string);

	str_repair(string, new);

	TEST_ASSERT_EQUAL_STRING("Hello, wOrld!", string);

	free(new);
}

void test_cipher_caesar_parse_key(void)
{
	char text[] = "Invalid key!";
	char text2[] = "12";
	char text3[] = "26";
	char text4[] = "27";

	unsigned int key = 0;
	bool success;

	TEST_ASSERT_EQUAL_INT(false, caesar_parse_key(text, &key));

	success = caesar_parse_key(text2, &key);
	TEST_ASSERT_EQUAL_INT(true, success);
	TEST_ASSERT_EQUAL_UINT(12, key);

	success = caesar_parse_key(text3, &key);
	TEST_ASSERT_EQUAL_INT(true, success);
	TEST_ASSERT_EQUAL_UINT(26, key);

	TEST_ASSERT_EQUAL_INT(false, caesar_parse_key(text4, &key));
}

void test_cipher_caesar_solve(void)
{
	char text[] = "AOPZPZHJHLZHYJPWOLY";
	char dest[20];
	caesar_solve(19, text, dest);

	TEST_ASSERT_EQUAL_STRING("THISISACAESARCIPHER", dest);
}

void test_cipher_caesar_crack(void)
{
	// Load the test data
	struct text_scorer scorer;
	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	char text[] = "AOPZPZHJHLZHYJPWOLY";
	char decoded[500];
	unsigned int used_key = caesar_crack(&scorer, text, decoded);

	TEST_ASSERT_EQUAL_STRING("THISISACAESARCIPHER", decoded);
	TEST_ASSERT_EQUAL_UINT(19, used_key);

	char text2[] = "HZOCJYDIRCDXCZVXCGZOOZMDIOCZKGVDIOZSODNMZKGVXZYWTVGZOOZMNJHZADSZYIPHWZMJA"
		"KJNDODJINYJRIOCZVGKCVWZOOCZHZOCJYDNIVHZYVAOZMEPGDPNXVZNVMRCJPNZYDODICDNKMDQVOZXJMMZNKJIYZIXZ";

	used_key = caesar_crack(&scorer, text2, decoded);

	TEST_ASSERT_EQUAL_STRING("METHODINWHICHEACHLETTERINTHEPLAINTEXTISREPLACEDBYALETTERSOMEFIXEDNUMBEROF"
		"POSITIONSDOWNTHEALPHABETTHEMETHODISNAMEDAFTERJULIUSCAESARWHOUSEDITINHISPRIVATECORRESPONDENCE",
		decoded);
	TEST_ASSERT_EQUAL_UINT(5, used_key);
}

void test_cipher_substitution_solve(void)
{
	char text[] = "HELLOWORLD";
	char key[] = "ZGIMTQFCPXDBVUNRKEWHOSJYAL";

	substitution_solve(key, text);
	TEST_ASSERT_EQUAL_STRING("CTBBNJNEBM", text);
}

void test_cipher_substitution_crack(void)
{
	// Load the test data
	struct text_scorer scorer;
	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	char text[] = "SOWFBRKAWFCZFSBSCSBQITBKOWLBFXTBKOWLSOXSOXFZWWIBICFWUQLRXINOCIJLWJFQUNWXLFBSZXFBT"
				  "XAANTQIFBFSFQUFCZFSBSCSBIMWHWLNKAXBISWGSTOXLXTSWLUQLXJBUUWLWISTBKOWLSWGSTOXLXTSWL"
				  "BSJBUUWLFULQRTXWFXLTBKOWLBISOXSSOWTBKOWLXAKOXZWSBFIQSFBRKANSOWXAKOXZWSFOBUSWJBSBF"
				  "TQRKAWSWANECRZAWJ";

	char *key = substitution_crack(&scorer, text);

	TEST_ASSERT_EQUAL_STRING("THESIMPLESUBSTITUTIONCIPHERISACIPHERTHATHASBEENINUSEFORMANYHUNDREDSOFYEARSITBASIC"
							 "ALLYCONSISTSOFSUBSTITUTINGEVERYPLAINTEXTCHARACTERFORADIFFERENTCIPHERTEXTCHARACTER"
							 "ITDIFFERSFROMCAESARCIPHERINTHATTHECIPHERALPHABETISNOTSIMPLYTHEALPHABETSHIFTEDITIS"
							 "COMPLETELYJUMBLED", text);

	TEST_ASSERT_EQUAL_STRING("LIUZJSXVNDPRGYHQOMTCFKEAWB", key);

	free(key);
}

int main(void)
{
	srand(5);
	UNITY_BEGIN();
	RUN_TEST(test_get_ngram_index);
	RUN_TEST(test_load_quadgrams);
	RUN_TEST(test_scorer_quadgram_score);
	RUN_TEST(test_string_case_change);
	RUN_TEST(test_string_schema);
	RUN_TEST(test_cipher_caesar_parse_key);
	RUN_TEST(test_cipher_caesar_solve);
	RUN_TEST(test_cipher_caesar_crack);
	RUN_TEST(test_cipher_substitution_solve);
	RUN_TEST(test_cipher_substitution_crack);
	return UNITY_END();
}

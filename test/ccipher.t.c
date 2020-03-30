#include <stdbool.h>
#include <stdlib.h>
#include <unity.h>
#include <libccipher/ccipher.h>
#include <libccipher/scorer.h>
#include <libccipher/string.h>
#include <libccipher/ciphers/caesar.h>
#include <libccipher/ciphers/substitution.h>
#include <libccipher/ciphers/vigenere.h>

void setUp()
{
	srand(5);
}

void tearDown()
{}

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

void test_cipher_caesar_with_data(void)
{
	// Load the quadgram data
	struct text_scorer scorer;
	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	// Test autocrack

	char text[] = "AOPZPZHJHLZHYJPWOLY";
	char result[sizeof(text)];
	char key[8];

	struct cipher_data data = {
			.ct = text,
			.key = key,
			.use_autocrack = true,
			.scorer = &scorer,
			.result = result,
	};

	caesar_with_data(&data);

	TEST_ASSERT_EQUAL_INT(true, data.success);
	TEST_ASSERT_EQUAL_STRING("19", data.key);
	TEST_ASSERT_EQUAL_STRING("THISISACAESARCIPHER", data.result);

	// Test solve with key

	char result2[sizeof(text)];

	struct cipher_data data2 = {
			.ct = text,
			.key = "19",
			.use_autocrack = false,
			.scorer = &scorer,
			.result = result2,
	};

	caesar_with_data(&data2);

	TEST_ASSERT_EQUAL_INT(true, data2.success);
	TEST_ASSERT_EQUAL_STRING("THISISACAESARCIPHER", data2.result);
}

void test_cipher_substitution_with_data(void)
{
	// Load the quadgram data
	struct text_scorer scorer;
	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	// Test autocrack

	char text[] = "SOWFBRKAWFCZFSBSCSBQITBKOWLBFXTBKOWLSOXSOXFZWWIBICFWUQLRXINOCIJLWJFQUNWXLFBSZXFBT"
				  "XAANTQIFBFSFQUFCZFSBSCSBIMWHWLNKAXBISWGSTOXLXTSWLUQLXJBUUWLWISTBKOWLSWGSTOXLXTSWL"
				  "BSJBUUWLFULQRTXWFXLTBKOWLBISOXSSOWTBKOWLXAKOXZWSBFIQSFBRKANSOWXAKOXZWSFOBUSWJBSBF"
				  "TQRKAWSWANECRZAWJ";
	char result[sizeof(text)];
	char key[27];

	struct cipher_data data = {
			.ct = text,
			.key = key,
			.use_autocrack = true,
			.scorer = &scorer,
			.result = result,
	};

	substitution_with_data(&data);

	TEST_ASSERT_EQUAL_INT(true, data.success);
	TEST_ASSERT_EQUAL_STRING("LIUZJSXVNDPRGYHQOMTCFKEAWB", data.key);
	TEST_ASSERT_EQUAL_STRING("THESIMPLESUBSTITUTIONCIPHERISACIPHERTHATHASBEENINUSEFORMANYHUNDREDSOFYEARSITBASIC"
							 "ALLYCONSISTSOFSUBSTITUTINGEVERYPLAINTEXTCHARACTERFORADIFFERENTCIPHERTEXTCHARACTER"
							 "ITDIFFERSFROMCAESARCIPHERINTHATTHECIPHERALPHABETISNOTSIMPLYTHEALPHABETSHIFTEDITIS"
							 "COMPLETELYJUMBLED", data.result);

	// Test solve with key

	char result2[sizeof(text)];

	struct cipher_data data2 = {
			.ct = text,
			.key = "LIUZJSXVNDPRGYHQOMTCFKEAWB",
			.use_autocrack = false,
			.scorer = &scorer,
			.result = result2,
	};

	substitution_with_data(&data2);

	TEST_ASSERT_EQUAL_INT(true, data2.success);
	TEST_ASSERT_EQUAL_STRING("THESIMPLESUBSTITUTIONCIPHERISACIPHERTHATHASBEENINUSEFORMANYHUNDREDSOFYEARSITBASIC"
							 "ALLYCONSISTSOFSUBSTITUTINGEVERYPLAINTEXTCHARACTERFORADIFFERENTCIPHERTEXTCHARACTER"
							 "ITDIFFERSFROMCAESARCIPHERINTHATTHECIPHERALPHABETISNOTSIMPLYTHEALPHABETSHIFTEDITIS"
							 "COMPLETELYJUMBLED", data2.result);
}

void test_cipher_substitution_parse_key(void)
{
	char text[] = "HLWJNOKMPVEUDFZTBGACRYIQXS";
	char text2[] = "HLWJNOKMPVEUDFZTBGACRYIQXSPPAS";
	char text3[] = "ABCBEFGHIJKLMNOPQRSTUVWXYZ";
	char text4[] = "ABCDEFG";
	char text5[] = "pemwgifcohdjvqrsyxlzbtunak";

	char key[27];
	bool success;

	success = substitution_parse_key(text, key);
	TEST_ASSERT_EQUAL_INT(true, success);
	TEST_ASSERT_EQUAL_STRING("HLWJNOKMPVEUDFZTBGACRYIQXS", key);

	success = substitution_parse_key(text2, key);
	TEST_ASSERT_EQUAL_INT(false, success);

	success = substitution_parse_key(text3, key);
	TEST_ASSERT_EQUAL_INT(false, success);

	success = substitution_parse_key(text4, key);
	TEST_ASSERT_EQUAL_INT(false, success);

	success = substitution_parse_key(text5, key);
	TEST_ASSERT_EQUAL_STRING("PEMWGIFCOHDJVQRSYXLZBTUNAK", key);
	TEST_ASSERT_EQUAL_INT(true, success);
}

void test_cipher_substitution_solve(void)
{
	char text[] = "HELLOWORLD";
	char key[] = "ZGIMTQFCPXDBVUNRKEWHOSJYAL";
	char dest[50];

	substitution_solve(key, text, dest);
	TEST_ASSERT_EQUAL_STRING("CTBBNJNEBM", dest);
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
	char dest[sizeof(text)];

	char *key = substitution_crack(&scorer, text, dest);

	TEST_ASSERT_EQUAL_STRING("THESIMPLESUBSTITUTIONCIPHERISACIPHERTHATHASBEENINUSEFORMANYHUNDREDSOFYEARSITBASIC"
							 "ALLYCONSISTSOFSUBSTITUTINGEVERYPLAINTEXTCHARACTERFORADIFFERENTCIPHERTEXTCHARACTER"
							 "ITDIFFERSFROMCAESARCIPHERINTHATTHECIPHERALPHABETISNOTSIMPLYTHEALPHABETSHIFTEDITIS"
							 "COMPLETELYJUMBLED", dest);

	TEST_ASSERT_EQUAL_STRING("LIUZJSXVNDPRGYHQOMTCFKEAWB", key);

	free(key);
}

void test_cipher_vigenere_solve(void)
{
	char text[] = "ISWXVIBJEXIGGBOCEWKBJEVIGGQS";
	char key[] = "FORTIFICATION";
	char dest[sizeof(text)];

	vigenere_solve(key, text, dest);

	TEST_ASSERT_EQUAL_STRING("DEFENDTHEEASTWALLOFTHECASTLE", dest);
}

void test_cipher_vigenere_crack(void)
{
	// Load the quadgram data
	struct text_scorer scorer;
	scorer_load_data(&scorer, fopen("./english_quadgrams.txt", "r"));

	char text[] = ""
				  "QRBAIUWYOKILBRZXTUWLEGXSNVDXWRXMHXYFCGMWWWSMELSXUZ"
				  "MKMFSBNZIFYEIEGRFZRXWKUFAXQEDXDTTHYNTBRJLHTAIKOCZX"
				  "QHBNDZIGZGPXARJEDYSJNUMKIFLBTNHWISWNVLFMEGXAIAAWSL"
				  "FMHXRSGRIGHEQTUMLGLVBRSILAEZSGXCMHTOWHFMLWMRKHPRFB"
				  "ELWGFRUGPBHNBEMKBNVWHHUEAKILBNBMLHKXUGMLYQKHPRFBEL"
				  "EJYNVWSIJBGAXGOTPMXRTXFKIWUALBRGWIEGHWHGAMEWWLTAEL"
				  "NUMREUWTBLSDPRLYVRETLEEDFROBEQUXTHXZYOZBXLKACKSOHN"
				  "VWXKSMAEPHIYQMMFSECHRFYPBBSQTXTPIWHGPXQDFWTAIKNNBX"
				  "SIYKETXTLVBTMQALAGHGOTPMXRTXTHXSFYGWMVKHLOIVUALMLD"
				  "LTSYVWYNVWMQVXPXRVYABLXDLXSMLWSUIOIIMELISOYEBHPHNR"
				  "WTVUIAKEYGWIETGWWBVMVDUMAEPAUAKXWHKMAUPAMUKHQPWKCX"
				  "EFXGWWSDDEOMLWLNKMWDFWTAMFAFEAMFZBNWIHYALXRWKMAMIK"
				  "GNGHJUAZHMHGUALYSULAELYHJBZMSILAILHWWYIKEWAHNPMLBN"
				  "NBVPJXLBEFWRWGXKWIRHXWWGQHRRXWIOMFYCZHZLVXNVIOYZCM"
				  "YDDEYIPWXTMMSHSVHHXZYEWNVOAOELSMLSWKXXFXSTRVIHZLEF"
				  "JXDASFIE";

	char dest[sizeof(text)];

	char *used_key = vigenere_crack(&scorer, text, dest);

	TEST_ASSERT_EQUAL_STRING("UNITEDSTATES", used_key);
	TEST_ASSERT_EQUAL_STRING("WETHEREFORETHEREPRESENTATIVESOFTHEUNITEDSTATESOFAMERICA"
							 "INGENERALCONGRESSASSEMBLEDAPPEALINGTOTHESUPREMEJUDGEOF"
							 "THEWORLDFORTHERECTITUDEOFOURINTENTIONSDOINTHENAME"
							 "ANDBYAUTHORITYOFTHEGOODPEOPLEOFTHESECOLONIESSOLEMNLYPUBLISH"
							 "ANDDECLARETHATTHESEUNITEDCOLONIESAREANDOFRIGHTOUGHTTOBE"
							 "FREEANDINDEPENDENTSTATESTHATTHEYAREABSOLVEDFROMALLALLEGIANCE"
							 "TOTHEBRITISHCROWNANDTHATALLPOLITICALCONNECTIONBETWEENTHEMAND"
							 "THESTATEOFGREATBRITAINISANDOUGHTTOBETOTALLYDISSOLVEDANDTHAT"
							 "ASFREEANDINDEPENDENTSTATESTHEYHAVEFULLPOWERTOLEVYWAR"
							 "CONCLUDEPEACECONTRACTALLIANCESESTABLISHCOMMERCEANDTODOALL"
							 "OTHERACTSANDTHINGSWHICHINDEPENDENTSTATESMAYOFRIGHTDOANDFOR"
							 "THESUPPORTOFTHISDECLARATIONWITHAFIRMRELIANCEONTHEPROTECTION"
							 "OFDIVINEPROVIDENCEWEMUTUALLYPLEDGETOEACHOTHEROURLIVES"
							 "OURFORTUNESANDOURSACREDHONOR", dest);

	free(used_key);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_get_ngram_index);
	RUN_TEST(test_load_quadgrams);
	RUN_TEST(test_scorer_quadgram_score);
	RUN_TEST(test_string_case_change);
	RUN_TEST(test_string_schema);
	RUN_TEST(test_cipher_caesar_with_data);
	RUN_TEST(test_cipher_caesar_parse_key);
	RUN_TEST(test_cipher_caesar_solve);
	RUN_TEST(test_cipher_caesar_crack);
	RUN_TEST(test_cipher_substitution_with_data);
	RUN_TEST(test_cipher_substitution_parse_key);
	RUN_TEST(test_cipher_substitution_solve);
	RUN_TEST(test_cipher_substitution_crack);
	RUN_TEST(test_cipher_vigenere_solve);
	RUN_TEST(test_cipher_vigenere_crack);
	return UNITY_END();
}

#include <unity.h>
#include <ccipher/ccipher.h>

void setUp() {}
void tearDown() {}

void hello_should_return_success(void)
{
	TEST_ASSERT_EQUAL_INT(0, hello());
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(hello_should_return_success);
	return UNITY_END();
}
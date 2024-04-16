#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../ssd-cli-test/TestShell.cpp"

class MockTestShell : public TestShell {
public:
	MOCK_METHOD(void, write, (int, string), ());
};

TEST(TestCaseName, TestWrite) {
	MockTestShell testShell;
	EXPECT_CALL(testShell, write(10, "0xABDCDDDD"))
		.Times(1);
	
	testShell.write(10, "0xABDCDDDD");
}



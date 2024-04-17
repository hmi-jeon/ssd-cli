#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../ssd-cli-test/TestShell.cpp"

class MockShell : public ISSD {
public:
	MOCK_METHOD(string, read, (int lba), (override));
	MOCK_METHOD(void, write, (int lba, string data), (override));
};

class TestFixture : public testing::Test {
public:
	MockShell shell;
};

TEST_F(TestFixture, TestRead) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(1);

	testShell.read(10);
}

TEST_F(TestFixture, TestWrite) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(1);

	testShell.write(10, "0xAAAABBBB");
}

TEST_F(TestFixture, TestFullRead) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(100);

	testShell.fullread();
}

TEST_F(TestFixture, TestFullWrite) {
	
	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(100);

	testShell.fullwrite("0xAAAABBBB");
}

TEST_F(TestFixture, TestInputCommnad) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(1);
	EXPECT_CALL(shell, write).Times(1);

	testShell.inputCommand("read 10");
	testShell.inputCommand("write 10 0xAAAABBBB");
}


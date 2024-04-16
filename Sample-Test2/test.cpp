#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../ssd-cli-test/TestShell.cpp"

class MockShell : public TestShell {
public:
	MOCK_METHOD(string, read, (int lba), ());
	MOCK_METHOD(void, write, (int lba, string data), ());
	MOCK_METHOD(void, fullread, (), ());
	MOCK_METHOD(void, exit, (), ());
	MOCK_METHOD(void, help, (), ());

};

class TestFixture : public testing::Test {
public:
	MockShell shell;
};

TEST_F(TestFixture, TestRead) {
	EXPECT_CALL(shell, read(1)).Times(1);
	shell.read(1);
}

TEST_F(TestFixture, TestRealClassRead) {
	TestShell tShell;
	EXPECT_EQ(tShell.read(1), "0xAAAABBBB");
}

TEST_F(TestFixture, TestExit) {
	EXPECT_CALL(shell, exit).Times(1);
	shell.exit();
}

TEST_F(TestFixture, TestFullRead) {
	EXPECT_CALL(shell, fullread).Times(1);

	shell.fullread();

TEST_F(TestFixture, TestWrite) {
	EXPECT_CALL(shell, write(10, "0xAAAABBBB")).Times(1);

	shell.write(10, "0xAAAABBBB");
}

TEST_F(TestFixture, TestHelp) {
	EXPECT_CALL(shell, help).Times(1);

	shell.help();
}

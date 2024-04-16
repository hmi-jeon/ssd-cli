#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../ssd-cli-test/TestShell.cpp"

class MockShell : public TestShell {
public:
	MOCK_METHOD(void, read, (int lba), ());
	MOCK_METHOD(void, exit, (), ());
	MOCK_METHOD(void, write, (int lba, string data), ());
};

class TestFixture : public testing::Test {
public:
	MockShell shell;
};

TEST_F(TestFixture, TestRead) {
	EXPECT_CALL(shell, read).Times(1);

	shell.read(1);
}

TEST_F(TestFixture, ExitRead) {
	EXPECT_CALL(shell, exit).Times(1);

	shell.exit();
}

TEST_F(TestFixture, TestWrite) {
	EXPECT_CALL(shell, write(10, "0xAAAABBBB")).Times(1);

	shell.write(10, "0xAAAABBBB");
}

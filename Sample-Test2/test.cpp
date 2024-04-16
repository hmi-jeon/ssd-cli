#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../ssd-cli-test/TestShell.cpp"

class MockShell : public TestShell {
public:
	MOCK_METHOD(void, read, (int pos), (override));
	MOCK_METHOD(void, exit, (), (override));
	MOCK_METHOD(void, write, (int LBA, string data), (override));
};

class TestFixture : public testing::Test {
public:
	MockShell shell;
};

TEST_F(TestFixture, TestRead) {
	MockShell shell;

	EXPECT_CALL(shell, read).Times(1);

	shell.read(1);
}

TEST_F(TestFixture, ExitRead) {
	MockShell shell;

	EXPECT_CALL(shell, exit).Times(1);

	shell.exit();
}

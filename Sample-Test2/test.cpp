#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../ssd-cli-test/TestShell.cpp"

class MockShell : public ISSD {
public:
	MOCK_METHOD(string, read, (int lba), (override));
	MOCK_METHOD(void, write, (int lba, string data), (override));
};

class TestShellFixture : public testing::Test {
public:
	MockShell shell;
};

TEST_F(TestShellFixture, read_count_once) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(1);

	testShell.read(10);
}

TEST_F(TestShellFixture, write_count_once) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(1);

	testShell.write(10, "0xAAAABBBB");
}

TEST_F(TestShellFixture, fullRead_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(100);

	testShell.fullread();
}

TEST_F(TestShellFixture, fullWrite_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(100);

	testShell.fullwrite("0xAAAABBBB");
}

TEST_F(TestShellFixture, TestInputCommnad) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(101);
	EXPECT_CALL(shell, write).Times(101);

	testShell.inputCommand("read 10");
	testShell.inputCommand("write 10 0xAAAABBBB");
	testShell.inputCommand("fullread");
	testShell.inputCommand("fullwrite 0xAAAABBBB");
}


TEST_F(TestShellFixture, TestInputCommnadExit) {

	TestShell testShell(&shell);

	testShell.exit();

	EXPECT_EQ(false, testShell.getStatus());
}

TEST_F(TestShellFixture, testApp1_read_write_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(100);
	EXPECT_CALL(shell, read).Times(100);
	testShell.testApp1();
}

TEST_F(TestShellFixture, testApp2_read_write_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(186);
	// called once becaue of return false
	EXPECT_CALL(shell, read).Times(1);
	testShell.testApp2();
}

TEST_F(TestShellFixture, Test_OneArgumentCommand) {

	TestShell testShell(&shell);

	testShell.inputCommand("exit 1");

	EXPECT_EQ(false, testShell.getIsValid());
}

TEST_F(TestShellFixture, Test_TwoArgumentCommand) {

	TestShell testShell(&shell);

	testShell.inputCommand("read");

	EXPECT_EQ(false, testShell.getIsValid());
}

TEST_F(TestShellFixture, Test_ThreeArgumentCommand) {

	TestShell testShell(&shell);

	testShell.inputCommand("write 10");

	EXPECT_EQ(false, testShell.getIsValid());
}

TEST_F(TestShellFixture, TestInvalidCommnad) {

	TestShell testShell(&shell);

	testShell.inputCommand("woite 10");

	EXPECT_EQ(false, testShell.getIsValid());
}
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

TEST_F(TestFixture, read_count_once) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(1);

	testShell.read(10);
}

TEST_F(TestFixture, write_count_once) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(1);

	testShell.write(10, "0xAAAABBBB");
}

TEST_F(TestFixture, fullRead_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, read).Times(100);

	testShell.fullread();
}

TEST_F(TestFixture, fullWrite_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(100);

	testShell.fullwrite("0xAAAABBBB");
}


TEST_F(TestFixture, testApp1_read_write_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(100);
	EXPECT_CALL(shell, read).Times(100);
	testShell.testApp1();
}

TEST_F(TestFixture, testApp2_read_write_count) {

	TestShell testShell(&shell);

	EXPECT_CALL(shell, write).Times(186);
	// called once becaue of return false
	EXPECT_CALL(shell, read).Times(1);
	testShell.testApp2();
}

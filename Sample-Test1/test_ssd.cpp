#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/ssd.cpp"
#include "../ssd-cli/virtual_nand.cpp"
#include <fstream>
#include <windows.h>

using namespace testing;

class MockNand : public lNAND {
public:
	MOCK_METHOD(void, read, (int), (override));
	MOCK_METHOD(void, write, (int, string), (override));
};

class SsdTest : public Test {
public:
protected:
	MockNand mockNand;
};

TEST_F(SsdTest, TestMockReadInvalidLBA) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, read(101))
		.Times(0);

	ssd.read(101);
}

TEST_F(SsdTest, TestMockRead) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, read(5))
		.Times(1);

	ssd.read(5);
}

TEST_F(SsdTest, TestMockWriteInvalidLBA) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(101, "12345667"))
		.Times(0);

	ssd.write(101, "0x12345667");
}

TEST_F(SsdTest, TestMockWriteInvalidValueSize) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12"))
		.Times(0);

	ssd.write(5, "0x12");
}

TEST_F(SsdTest, TestMockWriteInvalidValueHex) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "1234566Z"))
		.Times(0);

	ssd.write(5, "0x1234566Z");
}

TEST_F(SsdTest, TestMockWriteInvalidValuePrefix) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12345667"))
		.Times(0);

	ssd.write(5, "0b12345667");
}

TEST_F(SsdTest, TestMockWrite) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12345667"))
		.Times(1);

	ssd.write(5, "0x12345667");
}
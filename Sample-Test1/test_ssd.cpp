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
	VirtualNAND vnand;
};

class SsdMockTest : public Test {
public:
protected:
	MockNand mockNand;
};

TEST_F(SsdTest, TestValidLba) {
	EXPECT_TRUE(SSD(&vnand).isValidLba(5));
}

TEST_F(SsdTest, TestInValidLba) {
	EXPECT_FALSE(SSD(&vnand).isValidLba(101));
}

TEST_F(SsdTest, TestValidValue) {
	EXPECT_TRUE(SSD(&vnand).isValidValue("0x14329589"));
}

TEST_F(SsdTest, TestInValidValueSize) {
	EXPECT_FALSE(SSD(&vnand).isValidValue("a"));
}

TEST_F(SsdTest, TestInValidValue0x) {
	EXPECT_FALSE(SSD(&vnand).isValidValue("0b123410af"));
}

TEST_F(SsdTest, TestInValidValueHex) {
	EXPECT_FALSE(SSD(&vnand).isValidValue("0x0012341Z"));
}

TEST_F(SsdMockTest, TestMockReadInvalid) {
	EXPECT_CALL(mockNand, read(101))
		.Times(0);

	SSD(&mockNand).read(101);
}

TEST_F(SsdMockTest, TestMockRead) {
	EXPECT_CALL(mockNand, read(5))
		.Times(1);

	SSD(&mockNand).read(5);
}

TEST_F(SsdMockTest, TestMockWriteInvlaid) {
	EXPECT_CALL(mockNand, write(101, "12345667"))
		.Times(0);

	SSD(&mockNand).write(101, "0x12345667");
}

TEST_F(SsdMockTest, TestMockWrite) {
	EXPECT_CALL(mockNand, write(5, "12345667"))
		.Times(1);

	SSD(&mockNand).write(5, "0x12345667");
}
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

TEST(MockTest, TestMockReadInvalid) {
	MockNand mockNand;
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, read(101))
		.Times(0);

	ssd.read(101);
}

TEST(MockTest, TestMockRead) {
	MockNand mockNand;
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, read(5))
		.Times(1);

	ssd.read(5);
}

TEST(MockTest, TestMockWriteInvlaid) {
	MockNand mockNand;
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(101, "12345667"))
		.Times(0);

	ssd.write(101, "0x12345667");
}

TEST(MockTest, TestMockWrite) {
	MockNand mockNand;
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12345667"))
		.Times(1);

	ssd.write(5, "0x12345667");
}
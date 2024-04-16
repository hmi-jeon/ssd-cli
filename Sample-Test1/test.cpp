#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/ssd.cpp"
#include <fstream>
#include <windows.h>

using namespace testing;

class NandTest : public Test {
	void SetUp() override {
		NAND nand;
	}

	void TearDown() override {

	}
};

TEST_F(NandTest, CheckFileExist) {
	FILE* file;
	fopen_s(&file, "nand.txt", "r");
	EXPECT_THAT(file, Ne(nullptr));
}

TEST(SsdTest, TestValidLba) {
	NAND nand;
	SSD ssd(&nand);

	EXPECT_TRUE(ssd.isValidLba(5));
}

TEST(SsdTest, TestInValidLba) {
	NAND nand;
	SSD ssd(&nand);

	EXPECT_FALSE(ssd.isValidLba(101));
}
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/nand.cpp"
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
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/nand.cpp"
#include <fstream>
#include <windows.h>


using namespace testing;

class NandTest : public Test {
public:
	NAND nand;
	void SetUp() override {

	}

	void TearDown() override {

	}
};

TEST_F(NandTest, CheckFileExist) {
	FILE* file;
	fopen_s(&file, "nand.txt", "r");
	EXPECT_THAT(file, Ne(nullptr));
}

TEST_F(NandTest, ReadZerotoCheckResultFile)
{
	//NAND nand;
	nand.read(0);
	FILE* file;
	fopen_s(&file, "result.txt", "r");

	EXPECT_NE(file, nullptr);
	if (file != nullptr) fclose(file);
}

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/nand.cpp"
#include <fstream>
#include <windows.h>

using namespace testing;

class NandTest : public Test {
public:
	void SetUp() override {
	}

	void TearDown() override {
	}
protected:
	NAND nand;
};

TEST_F(NandTest, CheckFileExist) {
	FILE* file;
	fopen_s(&file, "nand.txt", "r");
	EXPECT_NE(file, nullptr);
	fclose(file);
}

TEST_F(NandTest, CheckWriteTest) {
	nand.write(2, "12345678");

	FILE* file;
	fopen_s(&file, "nand.txt", "r");
	fseek(file, 16, SEEK_SET);
	char buffer[9] = {};
	fread_s(buffer, 8, 1, 8, file);
	string s = buffer;
	EXPECT_EQ(s, string("12345678"));
	fclose(file);
}
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

TEST_F(NandTest, ReadZerotoCheckResultFile)
{
	//NAND nand;
	nand.read(0);
	FILE* file;
	fopen_s(&file, "result.txt", "r");

	EXPECT_NE(file, nullptr);
	if (file != nullptr) fclose(file);
}

TEST_F(NandTest, WriteAndReadOneAddr)
{
	string testString = "11223344";
	nand.write(10, testString.c_str());
	nand.read(10);
	FILE* file;
	fopen_s(&file, "result.txt", "r");
	char readData[9];
	fread(readData, 1, 8, file);
	readData[8] = '\0';
	EXPECT_EQ(readData, testString);
}
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/ssd.cpp"
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

TEST(SsdTest, TestValidValue) {
	NAND nand;
	SSD ssd(&nand);

	EXPECT_TRUE(ssd.isValidValue("0x14329589"));
}

TEST(SsdTest, TestInValidValue) {
	NAND nand;
	SSD ssd(&nand);

	EXPECT_FALSE(ssd.isValidValue("a"));
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

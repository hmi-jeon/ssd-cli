#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/ssd.cpp"
#include <fstream>
#include <windows.h>

using namespace testing;

class MockNand : public lNAND {
public:
	MockNand() {

	}

	MOCK_METHOD(void, read, (int), (override));
	MOCK_METHOD(void, write, (int, string), (override));
};


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

TEST(SsdTest, WriteCommand) {
	MockNand nand;
	SSD ssd(&nand);
	char* cmd[4] = {"appname", "W","20","0x12345678" };

	EXPECT_CALL(nand, write(_, _))
		.Times(1);
	ssd.command(4, cmd);
}

TEST(SsdTest, ReadCommand) {
	MockNand nand;
	SSD ssd(&nand);
	char* cmd[3] = {"appname", "R","20" };

	EXPECT_CALL(nand, read(_))
		.Times(1);
	ssd.command(3, cmd);
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

TEST(MockTest, TestMockRead) {
	MockNand mockNand;
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, read(5))
		.Times(1);

	ssd.read(5);
}

TEST(MockTest, TestMockWrite) {
	MockNand mockNand;
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12345667"))
		.Times(1);

	ssd.write(5, "0x12345667");
}
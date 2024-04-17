#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/virtual_nand.cpp"
#include <fstream>
#include <windows.h>

using namespace testing;

class VirtualNandTest : public Test {
public:
	void SetUp() override {

	}

	void TearDown() override {
		if (fs.is_open()) {
			fs.close();
		}
	}

protected:
	VirtualNAND nand;
	fstream fs;
};

TEST_F(VirtualNandTest, CheckFileExist) {
	fs.open(nand.NAND_FILE_NAME, ios_base::in);
	EXPECT_EQ(fs.is_open(), true);
}

TEST_F(VirtualNandTest, CheckWriteTest) {
	int lba = 2;
	string TEST_DATA = "12345678";
	nand.write(lba, TEST_DATA);

	fs.open(nand.NAND_FILE_NAME, ios_base::in);
	fs.seekg(lba * VirtualNAND::LBA_SIZE, ios_base::beg);
	char buffer[VirtualNAND::LBA_SIZE + 1] = {};
	fs.read(buffer, VirtualNAND::LBA_SIZE);
	EXPECT_EQ(string(buffer), TEST_DATA);
}

TEST_F(VirtualNandTest, ReadZerotoCheckResultFile)
{
	nand.read(0);
	fs.open(nand.NAND_FILE_NAME, ios_base::in);
	EXPECT_EQ(fs.is_open(), true);
}

TEST_F(VirtualNandTest, WriteAndReadOneAddr)
{
	string TEST_DATA = "0x11223344";
	int lba = 10;
	nand.write(lba, TEST_DATA.c_str()+2);
	nand.read(lba);
	fs.open(nand.RESULT_FILE_NAME, ios_base::in);
	char readData[VirtualNAND::LBA_SIZE + 3] = {};
	fs.read(readData, VirtualNAND::LBA_SIZE + 3);
	readData[VirtualNAND::LBA_SIZE + 2] = '\0';
	EXPECT_EQ(string(readData), TEST_DATA);
}
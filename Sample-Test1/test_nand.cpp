#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/virtual_nand.cpp"
#include <fstream>
#include <windows.h>

using namespace testing;

class VirtualNandTest : public Test {
public:
	void SetUp() override {
		strNandFileName = nand.getNandFileName();
		LBA_SIZE = nand.getLBASize();
	}

	void TearDown() override {
		if (fs.is_open()) {
			fs.close();
		}
	}

protected:
	VirtualNAND nand;
	fstream fs;
	string strNandFileName;
	int LBA_SIZE;
};

TEST_F(VirtualNandTest, CheckFileExist) {
	fs.open(strNandFileName, ios_base::in);
	EXPECT_EQ(fs.is_open(), true);
}

TEST_F(VirtualNandTest, CheckWriteTest) {
	int lba = 2;
	string TEST_DATA = "12345678";
	nand.write(lba, TEST_DATA);

	fs.open(strNandFileName, ios_base::in);
	fs.seekg(lba * LBA_SIZE, ios_base::beg);
	char* data = (char*)malloc(LBA_SIZE + 1);
	fs.read(data, LBA_SIZE);
	data[LBA_SIZE] = '\0';
	EXPECT_EQ(string(data), TEST_DATA);
}

TEST_F(VirtualNandTest, WriteAndReadOneAddr)
{
	string TEST_DATA = "11223344";
	int lba = 10;
	nand.write(lba, TEST_DATA);
	EXPECT_EQ(nand.read(lba), TEST_DATA);
}
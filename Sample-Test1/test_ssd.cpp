#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd-cli/ssd.cpp"
#include "../ssd-cli/virtual_nand.cpp"
#include <fstream>
#include <windows.h>
#include <string>

using namespace std;
using namespace testing;

class MockNand : public INAND {
public:
	MOCK_METHOD(string, read, (const int), (override));
	MOCK_METHOD(void, write, (const int, const string), (override));
};

class SsdTest : public Test {
public:
	SsdTest(INAND* nand)
		:ssd(nand)
	{
	}

	void SetUp() override {

	}

	void TearDown() override {
		if (fs_.is_open()) {
			fs_.close();
		}
	}

protected:
	int LBA_SIZE;
	SSD ssd;
	fstream fs_;
};

class SsdMockNandTest : public SsdTest {
public:
	explicit SsdMockNandTest()
		: SsdTest(&nand)
	{
		LBA_SIZE = 8;
	}

protected:
	MockNand nand;
};

class SsdVirtualNandTest : public SsdTest {
public:
	explicit SsdVirtualNandTest()
		: SsdTest(&nand)
	{
		LBA_SIZE = nand.getLBASize();
	}

protected:
	VirtualNAND nand;
};

TEST_F(SsdMockNandTest, TestMockWriteCommand) {
	char* cmd[4] = { "appname", "W","20","0x12345678" };

	EXPECT_CALL(nand, write(_, _))
		.Times(1);
	ssd.command(4, cmd);
}

TEST_F(SsdMockNandTest, TestMockReadCommand) {
	char* cmd[3] = { "appname", "R","20" };

	EXPECT_CALL(nand, read(_))
		.Times(1);
	ssd.command(3, cmd);
}


TEST_F(SsdMockNandTest, TestMockReadInvalidLBA) {
	EXPECT_CALL(nand, read(101))
		.Times(0);

	ssd.read(101);
}

TEST_F(SsdMockNandTest, TestMockRead) {
	string testString = "0x11223344";

	EXPECT_CALL(nand, read(5))
		.Times(1)
		.WillOnce(Return(testString.substr(2)));

	ssd.read(5);

	char* readData = (char*)malloc(LBA_SIZE + 3);
	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), testString);
}

TEST_F(SsdMockNandTest, TestMockWriteInvalidLBA) {
	EXPECT_CALL(nand, write(101, "12345667"))
		.Times(0);

	ssd.write(101, "0x12345667");
}

TEST_F(SsdMockNandTest, TestMockWriteInvalidValueSize) {
	EXPECT_CALL(nand, write(5, "12"))
		.Times(0);

	ssd.write(5, "0x12");
}

TEST_F(SsdMockNandTest, TestMockWriteInvalidValueHex) {
	EXPECT_CALL(nand, write(5, "1234566Z"))
		.Times(0);

	ssd.write(5, "0x1234566Z");
}

TEST_F(SsdMockNandTest, TestMockWriteInvalidValuePrefix) {
	EXPECT_CALL(nand, write(5, "12345667"))
		.Times(0);

	ssd.write(5, "0b12345667");
}

TEST_F(SsdMockNandTest, TestMockWrite) {
	EXPECT_CALL(nand, write(5, "12345667"))
		.Times(1);

	ssd.write(5, "0x12345667");
}

TEST_F(SsdVirtualNandTest, TestWriteAndRead) {
	string testString = "0x11223354";
	ssd.write(0, testString);
	ssd.read(0);

	char* readData = (char*)malloc(LBA_SIZE + 3);
	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), testString);
}

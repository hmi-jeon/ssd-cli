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

	int getLBASize() const { return 8; }

	int getMaxLBA() const { return 100; }
};

class SsdTest : public Test {
public:
	SsdTest(INAND* nand)
		:ssd(nand)
	{
	}

	void SetUp() override {
		ssd.init();
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
	ICommand* command;
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

TEST_F(SsdMockNandTest, DISABLED_TestMockWriteCommand) {
	char* cmd[4] = { "appname", "W","20","0x12345678" };

	EXPECT_CALL(nand, write(_, _))
		.Times(1);
	ssd.command(4, cmd);
}

TEST_F(SsdMockNandTest, DISABLED_TestMockReadCommand) {
	char* cmd[3] = { "appname", "R","20" };

	EXPECT_CALL(nand, read(_))
		.Times(1);
	ssd.command(3, cmd);
}

TEST_F(SsdMockNandTest, DISABLED_TestMockWriteInvalidCommand) {
	char* cmd[3] = { "appname", "W","20"};

	EXPECT_CALL(nand, write(_, _))
		.Times(0);
	ssd.command(3, cmd);
}

TEST_F(SsdMockNandTest, DISABLED_TestMockReadInvalidCommand) {
	char* cmd[3] = { "appname", "R", "fdas"};

	EXPECT_CALL(nand, read(_))
		.Times(0);
	ssd.command(3, cmd);
}

TEST_F(SsdMockNandTest, DISABLED_TestMockReadInvalidLBA) {
	EXPECT_CALL(nand, read(101))
		.Times(0);

	command = new Read();
	command->execute(vector<string>{"R", "101"}, &nand, WriteBuffer());
}

TEST_F(SsdMockNandTest, DISABLED_TestMockRead) {
	string testString = "0x11223344";

	EXPECT_CALL(nand, read(5))
		.Times(1)
		.WillOnce(Return(testString.substr(2)));

	command = new Read();
	command->execute(vector<string>{"R", "5"}, &nand, WriteBuffer());

	char* readData = (char*)malloc(LBA_SIZE + 3);
	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), testString);
}

TEST_F(SsdMockNandTest, DISABLED_TestMockWriteInvalidLBA) {
	EXPECT_CALL(nand, write(101, "12345667"))
		.Times(0);

	command = new Write();
	command->execute(vector<string>{"W", "101", "0x12345667"}, &nand, WriteBuffer());
}

TEST_F(SsdMockNandTest, DISABLED_TestMockWriteInvalidValueSize) {
	EXPECT_CALL(nand, write(5, "12"))
		.Times(0);

	command = new Write();
	command->execute(vector<string>{"W", "5", "0x12"}, &nand, WriteBuffer());
}

TEST_F(SsdMockNandTest, DISABLED_TestMockWriteInvalidValueHex) {
	EXPECT_CALL(nand, write(5, "1234566Z"))
		.Times(0);

	command = new Write();
	command->execute(vector<string>{"W", "5", "0x1234566Z"}, &nand, WriteBuffer());
}

TEST_F(SsdMockNandTest, DISABLED_TestMockWriteInvalidValuePrefix) {
	EXPECT_CALL(nand, write(5, "12345667"))
		.Times(0);

	command = new Write();
	command->execute(vector<string>{"W", "5", "0b12345667"}, &nand, WriteBuffer());
}

TEST_F(SsdMockNandTest, DISABLED_TestMockWrite) {
	EXPECT_CALL(nand, write(5, "12345667"))
		.Times(1);

	command = new Write();
	command->execute(vector<string>{"W", "5", "0x12345667"}, &nand, WriteBuffer());
}

TEST_F(SsdVirtualNandTest, DISABLED_TestWriteAndRead) {
	string testString = "0x11223354";

	ICommand* commandWrite = new Write();
	ICommand* commandRead = new Read();

	commandWrite->execute(vector<string>{"W", "0", testString}, &nand, WriteBuffer());
	commandRead->execute(vector<string>{"R", "0"}, &nand, WriteBuffer());

	char* readData = (char*)malloc(LBA_SIZE + 3);
	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), testString);
}

TEST_F(SsdVirtualNandTest, TestWriteAndReadSSD) {
	string testString = "0x11223354";

	const char* argv[] = { "app", "W", "1", testString.c_str() };
	ssd.command(4, (char**)argv);

	const char* argv2[] = { "app", "R", "1" };
	ssd.command(3, (char**)argv2);

	char* readData = (char*)malloc(LBA_SIZE + 3);
	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), testString);
}

TEST_F(SsdVirtualNandTest, TestWriteAndReadSSD_10times) {
	string testString = "0x11223354";

	for (int cnt = 0; cnt < 10; ++cnt) {
		const char* argv[] = { "app", "W", "1", testString.c_str() };
		ssd.command(4, (char**)argv);
	}

	const char* argv2[] = { "app", "R", "1" };
	ssd.command(3, (char**)argv2);

	char* readData = (char*)malloc(LBA_SIZE + 3);
	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), testString);
}

TEST_F(SsdVirtualNandTest, TestEraseSSD) {
	string testString = "0x11223354";

	char* argv[] = { "app", "W", "3", (char*)testString.c_str() };
	ssd.command(4, (char**)argv);

	char* argv2[] = { "app", "R", "3" };
	ssd.command(3, argv2);

	char* readData = (char*)malloc(LBA_SIZE + 3);
	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	fs_.close();
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), testString);

	char* argv3[] = { "app", "E", "3", "5"};
	ssd.command(4, argv3);
	ssd.command(3, argv2);

	fs_.open(ssd.getResultFileName(), ios_base::in);
	fs_.read(readData, LBA_SIZE + 2);
	fs_.close();
	readData[LBA_SIZE + 2] = '\0';

	EXPECT_EQ(string(readData), string("0x00000000"));
}

TEST_F(SsdVirtualNandTest, TestFlushSSD) {
	string testString = "0x11223354";

	char* argv[] = { "app", "E", "0", "5" };
	ssd.command(4, argv);

	char* argv2[] = { "app", "F" };
	ssd.command(2, argv2);

	char* argv3[] = { "app", "W", "2", (char*)testString.c_str() };
	ssd.command(4, argv3);

	char* nandData = (char*)malloc(LBA_SIZE + 1);
	fs_.open("nand.txt", ios_base::in);
	fs_.seekg(2 * LBA_SIZE, ios_base::beg);
	fs_.read(nandData, LBA_SIZE);
	fs_.close();
	nandData[LBA_SIZE] = '\0';

	EXPECT_EQ(string(nandData), string("00000000"));
	
	ssd.command(2, argv2);

	fs_.open("nand.txt", ios_base::in);
	fs_.seekg(2 * LBA_SIZE, ios_base::beg);
	fs_.read(nandData, LBA_SIZE);
	fs_.close();
	nandData[LBA_SIZE] = '\0';

	EXPECT_EQ(string(nandData), testString.substr(2));
}



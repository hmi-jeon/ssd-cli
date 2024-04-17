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

class SsdMockTest : public Test {
public:
protected:
	MockNand mockNand;
};

class SsdTest : public Test {
public:
protected:
	VirtualNAND vnand;
};

TEST_F(SsdMockTest, TestMockReadInvalidLBA) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, read(101))
		.Times(0);

	ssd.read(101);
}

TEST_F(SsdMockTest, TestMockRead) {
	SSD ssd(&mockNand);
	string testString = "0x11223344";

	EXPECT_CALL(mockNand, read(5))
		.Times(1)
		.WillOnce(Return(testString.substr(2)));

	ssd.read(5);

	const string fileName = "result.txt";
	char readData[VirtualNAND::LBA_SIZE + 3] = {};
	fstream fs;
	fs.open(fileName.c_str(), ios_base::in);
	fs.read(readData, VirtualNAND::LBA_SIZE + 3);
	readData[VirtualNAND::LBA_SIZE + 2] = '\0';
	fs.close();

	EXPECT_EQ(string(readData), testString);
}

TEST_F(SsdMockTest, TestMockWriteInvalidLBA) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(101, "12345667"))
		.Times(0);

	ssd.write(101, "0x12345667");
}

TEST_F(SsdMockTest, TestMockWriteInvalidValueSize) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12"))
		.Times(0);

	ssd.write(5, "0x12");
}

TEST_F(SsdMockTest, TestMockWriteInvalidValueHex) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "1234566Z"))
		.Times(0);

	ssd.write(5, "0x1234566Z");
}

TEST_F(SsdMockTest, TestMockWriteInvalidValuePrefix) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12345667"))
		.Times(0);

	ssd.write(5, "0b12345667");
}

TEST_F(SsdMockTest, TestMockWrite) {
	SSD ssd(&mockNand);

	EXPECT_CALL(mockNand, write(5, "12345667"))
		.Times(1);

	ssd.write(5, "0x12345667");
}

TEST_F(SsdTest, TestWriteAndRead) {
	SSD ssd(&vnand);
	string testString = "0x11223354";
	ssd.write(0, testString);
	ssd.read(0);

	const string fileName = "result.txt";
	char readData[VirtualNAND::LBA_SIZE + 3] = {};
	fstream fs;
	fs.open(fileName.c_str(), ios_base::in);
	fs.read(readData, VirtualNAND::LBA_SIZE + 3);
	readData[VirtualNAND::LBA_SIZE + 2] = '\0';
	fs.close();

	EXPECT_EQ(string(readData), testString);
}

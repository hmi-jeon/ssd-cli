#pragma once
#include <string>

using namespace std;

class lNAND {
public:
	virtual void read(int lba) = 0;
	virtual void write(int lba, string value) = 0;

protected:
	static constexpr int LBA_SIZE = 8;
	static constexpr int MAX_LBA = 100;
	static constexpr int NAND_SIZE = LBA_SIZE * MAX_LBA;
	const char NAND_FILE_NAME[12] = "nand.txt";
	const char RESULT_FILE_NAME[12] = "result.txt";
	const char FORMAT_DATA[LBA_SIZE + 1] = "00000000";
};
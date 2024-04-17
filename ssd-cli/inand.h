#pragma once
#include <string>

using namespace std;

class lNAND {
public:
	virtual void read(int lba) = 0;
	virtual void write(int lba, string value) = 0;

protected:
};
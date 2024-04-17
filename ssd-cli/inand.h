#pragma once
#include <string>

using namespace std;

class INAND {
public:
	virtual string read(const int lba) = 0;
	virtual void write(const int lba, const string value) = 0;

protected:
};
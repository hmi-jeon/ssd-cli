#pragma once
#include <vector>
#include <string>

using namespace std;

class ICommand {

public:
	virtual void execute() = 0;

protected :
	string name;
	vector<string> args;
};

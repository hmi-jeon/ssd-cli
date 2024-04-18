#pragma once
#include <vector>
#include <string>
#include "Logger.hpp"

using namespace std;

class ICommand {

public:
	virtual void execute() = 0;

protected :
	string name;
	vector<string> args;
	Logger& logger = Logger::getInstance();
};

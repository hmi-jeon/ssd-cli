#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <process.h>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Read.cpp"
#include "Write.cpp"
#include "Exit.cpp"
#include "Help.cpp"
#include "FullRead.cpp"
#include "FullWrite.cpp"
#include "Logger.cpp"


using namespace std;

interface ISSD {
	virtual string read(const int lba) = 0;
	virtual void write(const int lba, const string data) = 0;
};

class TestShell {
public:
	vector<string> parsingInput(const string inputString) {
		stringstream ss(inputString);
		vector<string> argList;
		string arg;

		while (getline(ss, arg, ' ')) {
			argList.push_back(arg);
		}

		if (argList.size() > 0) {
			transform(argList[0].begin(), argList[0].end(), argList[0].begin(), ::toupper);
		}

		return argList;
	}

	bool checkExistcommand(string command) {
		vector<string> commandList = { "WRITE", "READ", "EXIT" , "HELP", "FULLREAD", "FULLWRITE", "TESTAPP1", "TESTAPP2"};

		if (find(commandList.begin(), commandList.end(), command) == commandList.end()) {
			return false;
		}

		return true;
	}

	bool checkValidArguments(const vector<string> args) {
		if (args.size() < 1)  return false;

		string command = args[0];

		if (command == "EXIT" || command == "HELP" || command == "FULLREAD" || command == "TESTAPP1" || command == "TESTAPP2") {
			if (args.size() != 1) return false;
		}

		if (command == "READ") {
			if (args.size() != 2) return false;
			if (!_isValidLba(args[1])) return false;
		}

		if (command == "FULLWRITE") {
			if (args.size() != 2) return false;
			if (!_isValidValue(args[1])) return false;

		}

		if (command == "WRITE") {
			if (args.size() != 3) return false;
			if (!(_isValidLba(args[1]) && _isValidValue(args[2]))) return false;
		}

		return true;
	}

	bool checkInputValidation() {

		if (checkExistcommand(args[0]) == false) {
			return false;
		}

		if (checkValidArguments(args) == false) {
			return false;
		};

		return true;
	}

	void executeCommand() {
		string command = args[0];
		if (command == "TESTAPP1") {
			//TestApp1
			string appName = "FullWriteReadCompare.exe";
			if(system(appName.c_str())) cout << "FAIL";
			return;
		}
		if (command == "TESTAPP2") {
			//TestApp2
			string appName = "FullWriteReadCompare.exe";
			if (system(appName.c_str())) cout << "FAIL";
			return;
		}

		ICommand* icom{};
		if (command == "WRITE"    ) icom = new Write(args);
		if (command == "READ"     ) icom = new Read(args);
		if (command == "EXIT"     ) icom = new Exit(args);
		if (command == "HELP"     ) icom = new Help(args);
		if (command == "FULLREAD" )	icom = new FullRead(args);
		if (command == "FULLWRITE") icom = new FullWrite(args);

		icom->execute();
	}

	void inputCommand(const string userInput) {
		args = parsingInput(userInput);
		isValid = checkInputValidation();
		if (isValid == false) {
			_printInvalidCommand();
			return;
		}
		executeCommand();
	}
  
	bool getIsValid() {
		return isValid;
	}
protected:
	bool _isValidLba(const string lba) {
		for (char c : lba) {
			if (!std::isdigit(c)) {
				return false;
			}
		}
		int lbaDigit = stoi(lba);
		return (lbaDigit >= 0 && lbaDigit < 100);
	}

	bool _isValidValue(const string value) {
		if (value.size() != 10)
			return false;

		if (value.substr(0, 2) != "0x")
			return false;

		for (const char& c : value.substr(2)) {
			if (!isxdigit(c)) {
				return false;
			}
		}
		return true;
	}

	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}
	Logger& logger = Logger::getInstance();
	bool isValid = false;
	vector<string> args;
};

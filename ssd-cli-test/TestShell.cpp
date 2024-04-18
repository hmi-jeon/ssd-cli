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


	bool checkInputValidation() {

		if (_checkNumberOfArguments(args) == false) {
			return false;
		};

		if (_checkExistcommand(args[0]) == false) {
			return false;
		}

		return true;
	}

	void executeCommand() {
		string command = args[0];
		int lba = -1;
		if (args.size() >= 2) lba = stoi(args[1]);

		string data;
		if (args.size() == 3) data = args[2];

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



protected:
	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}	

	bool _getIsValid() {
		return isValid;
	}

	bool _checkExistcommand(string command) {
		vector<string> commandList = { "WRITE", "READ", "EXIT" , "HELP", "FULLREAD", "FULLWRITE", "TESTAPP1", "TESTAPP2" };

		if (find(commandList.begin(), commandList.end(), command) == commandList.end()) {
			return false;
		}

		return true;
	}

	bool _checkNumberOfArguments(const vector<string> args) {
		if (args.size() < 1)  return false;

		string command = args[0];

		if (command == "EXIT" || command == "HELP" || command == "FULLREAD" || command == "TESTAPP1" || command == "TESTAPP2") {
			if (args.size() != 1) return false;
		}

		if (command == "READ" || command == "FULLWRITE") {
			if (args.size() != 2) return false;
		}

		if (command == "WRITE") {
			if (args.size() != 3) return false;
		}

		return true;
	}

	bool isValid = false;
	vector<string> args;
};

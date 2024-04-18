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

#define RUN_LIST "run_list.lst"

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
		else if (command == "READ"     ) icom = new Read(args);
		else if (command == "EXIT"     ) icom = new Exit(args);
		else if (command == "HELP"     ) icom = new Help(args);
		else if (command == "FULLREAD" )	icom = new FullRead(args);
		else if (command == "FULLWRITE") icom = new FullWrite(args);

		isValid = icom->execute();
		if (isValid == false)
			_printInvalidCommand();
	}

	void inputCommand(const string userInput) {
		args = parsingInput(userInput);
		isValid = checkExistcommand(args[0]);
		if (isValid == false) {
			_printInvalidCommand();
			return;
		}
		executeCommand();
	}
  
	vector<string> getFlieData(string fileName) {
		string filename(fileName);
		vector<string> lines;
		string line;

		ifstream input_file(filename);

		if (!input_file.is_open()) return lines;

		while (getline(input_file, line)) {
			lines.push_back(line);
		}

		input_file.close();

		return lines;
	}

	void Runner() {
		vector<string> TestFileList = getFlieData(RUN_LIST);

		logger.setLoggerMode(RUNNER_MODE);

		int TestResult = 1;

		for (const string TestScenario : TestFileList) {

			TestResult = system(TestScenario.c_str());

			if (TestResult != 0) {
				logger.print(TestScenario + "\t---\tRun...Fail!");
				break;
			}

			logger.print(TestScenario + "\t---\tRun...Pass");
		}
	}

protected:
	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}
	Logger& logger = Logger::getInstance();
	bool isValid = false;
	vector<string> args;
};

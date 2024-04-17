#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <process.h>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

#define MAX_SIZE 100

using namespace std;

interface ISSD {
	virtual string read(const int lba) = 0;
	virtual void write(const int lba, const string data) = 0;
};

class ssdAPI : public ISSD {
public:
	string read(const int lba) override {
		// ssd.exe call
		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "R" + " " + to_string(lba);
		system(command.c_str());

		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open()) {
			resultFile >> data;
		}
		return data;
	};

	void write(const int lba, const string data) override {
		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "W" + " " + to_string(lba) + " " + data;
		system(command.c_str());
	};
};

class TestShell {
public:
	TestShell() {
		this->ssdAPI = nullptr;
	}

	TestShell(ISSD* ssdAPI) {
		this->ssdAPI = ssdAPI;
	}

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

	bool checkNumberOfArguments(const vector<string> args) {
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

	bool checkInputValidation() {

		if (checkNumberOfArguments(args) == false) {
			return false;
		};

		if (checkExistcommand(args[0]) == false) {
			return false;
		}

		return true;
	}

	void executeCommand() {
		string command = args[0];
		int lba = -1;
		if (args.size() >= 2) {
			try {
				lba = stoi(args[1]);
			}
			catch (exception& e) {
				_printInvalidCommand();
				return;
			}
		}

		string data;
		if (args.size() == 3) data = args[2];

		if (command == "WRITE") {
			ssdAPI->write(lba, data);
		}

		if (command == "READ") {
			ssdAPI->read(lba);
		}

		if (command == "EXIT") {
			exit();
		}

		if (command == "HELP") {
			help();
		}

		if (command == "FULLREAD") {
			fullread();
		}

		if (command == "FULLWRITE") {
			fullwrite(data);
		}

		if (command == "TESTAPP1") {
			testApp1();
		}

		if (command == "TESTAPP2") {
			testApp2();
		}
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

	string read(const int lba) {
		return ssdAPI->read(lba);;
	}

	void write(const int lba, const string data) {
		ssdAPI->write(lba, data);
	}

	void exit() {
		status = false;
	};

	void help() {
		for (int i = 0; i < helps.size(); i++) {
			cout << helps[i][0] << " : " << helps[i][1] << endl;
		}
	};

	vector<string> fullread() {
		vector<string> res;
		for (int lba = 0; lba < MAX_SIZE; lba++) {
			res.push_back(read(lba));
		}
		return res;
	};

	void fullwrite(const string data) {
		for (int lba = 0; lba < 100; lba++) {
			write(lba, data);
		}
	}

	bool getStatus() {
		return status;
	}

	bool testApp1() {
		vector<string> res;
		string testData = "0x12345678";
		fullwrite(testData);
		res = fullread();

		for (const string& data : res) {
			if (data != testData)
				return false;
		}
		return true;
	}

	bool testApp2() {
		string oldTestData = "0xAAAABBBB";
		string newTestData = "0x12345678";
		for (int i = 0; i < 30; i++) {
			for (int lba = 0; lba <= 5; lba++) {
				write(lba, oldTestData);
			}
		}
		for (int lba = 0; lba <= 5; lba++) {
			write(lba, newTestData);
		}

		for (int lba = 0; lba <= 5; lba++) {
			if (read(lba) != newTestData)
				return false;
		}
		return true;
	}

	bool getIsValid() {
		return isValid;
	}

protected:
	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}

	ISSD* ssdAPI;
	bool status = true;
	bool isValid = false;
	vector<string> args;
	vector<vector<string>> helps = {
		{ "READ","Outputs data written to the LBA address value of the device. (ex. READ 3)" },
		{ "WRITE","Records input data into the designated LBA of the device. (ex. WRITE 1 0x1234ABCD)" },
		{ "EXIT","Exit SHELL. (ex. EXIT)" },
		{ "HELP", "Prints SHELL's command list and help. (ex. HELP)" },
		{ "FULLREAD", "Reads the entire LBA of the device and outputs all data. (ex. FULLREAD)" },
		{ "FULLWRITE", "The input data is recorded in the entire LBA of the device. (ex. FULLWRITE 0x1234ABCD)" },
		{ "TESTAPP1","Test the read and write functions of the entire LBA of the device. (ex. TESTAPP1)" },
		{ "TESTAPP2","Test your device's ability to overwrite data. (ex. TESTAPP2)" }
	};
};


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

#define interface struct
#define MAX_SIZE 100

using namespace std;

interface ISSD {
	virtual string read(int lba) = 0;
	virtual void write(int lba, string data) = 0;
};

class ssdAPI : public ISSD {
public:
	string read(int lba) override {
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

  void write(int lba, string data) override {
		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "W" + " " + to_string(lba) + " " + data;
		system(command.c_str());
	};
};

class TestShell {
public:
	ISSD* ssdAPI;
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

	TestShell() {
		this->ssdAPI = nullptr;
	}

	TestShell(ISSD* ssdAPI) {
		this->ssdAPI = ssdAPI;
	}

	vector<string> parsingInput(string inputString) {
		stringstream ss(inputString);
		vector<string> argList;
		string arg;

		while (getline(ss, arg, ' ')) {
			argList.push_back(arg);
		}

		return argList;
	}

	bool checkExistcommand(string command) {
		vector<string> commandList = { "write", "read", "exit" , "help", "fullread", "fullwrite" };

		if (find(commandList.begin(), commandList.end(), command) == commandList.end()) {
			cout << "INVALID COMMAND" << endl;
			return false;
		}

		return true;
	}

	bool checkNumberOfArguments(vector<string> args) {
		if (args.size() < 1)  return false;

		string command = args[0];

		if (command == "exit" || command == "help" || command == "fullread") {
			if (args.size() != 1) return false;
		}

		if (command == "read" || command == "fullwrite") {
			if (args.size() != 2) return false;
		}

		if (command == "write") {
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
		if (args.size() >= 2) lba = stoi(args[1]);

		string data;
		if (args.size() == 3) data = args[2];

		if (command == "write") {
			ssdAPI->write(lba, data);
		}

		if (command == "read") {
			ssdAPI->read(lba);
		}

		if (command == "exit") {
			exit();
		}

		if (command == "help") {
			help();
		}

		if (command == "fullread") {
			fullread();
		}

		if (command == "fullwrite") {
			fullwrite(data);
		}
	}

	void inputCommand(string userInput) {
		args = parsingInput(userInput);
		if(checkInputValidation() == false) return;
		executeCommand();
	}

	string read(int lba) {
		return ssdAPI->read(lba);;
	}

	void write(int lba, string data) {
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

	void fullwrite(string data) {
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

		for (string data : res) {
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

private:
	bool status = true;
	vector<string> args;

};


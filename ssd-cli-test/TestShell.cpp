#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <process.h>
#include <sstream>
#include <vector>

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
		//system(ssd.exe read lba);

		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open()) {
			resultFile >> data;
			cout << data << endl;
		}
		return data;
	};

	void write(int lba, string data) override {
		string command = "ssd.exe W " + to_string(lba) + " " + data;
		system(command.c_str());
	};
};

class TestShell {
public:
	ISSD* ssdAPI;
	bool status{ true };
	string line{};
	int adr[100] = { 0 , };

	void inputCommand(string userInput) {
		stringstream ss(userInput);
		vector<string> words;
		string word;

		while (getline(ss, word, ' ')) {
			words.push_back(word);
		}

		if (words[0] == "write") {
			ssdAPI->write(10, "0xAAAABBBB");
		}

		if (words[0] == "read") {
			ssdAPI->read(10);
		}

		if (words[0] == "exit") {
		}

		if (words[0] == "help") {
		}

		if (words[0] == "fullread") {
			fullread();
		}

		if (words[0] == "fullwrite") {
			fullwrite("0xAAAABBBB");
		}
	}

	TestShell(ISSD* ssdAPI) {
		this->ssdAPI = ssdAPI;
	}

	string read(int lba) {
		ssdAPI->read(lba);
		return "";
	}

	void write(int lba, string data) {
		ssdAPI->write(lba, data);
	}

	void exit() {
	};

	void help() {
		cout << "--help--" << endl;
		cout << "--[write]--" << endl;
		cout << "--format: write [LBA] [DATA]--" << endl;
	};

	void fullread() {
		for (int lba = 0; lba < MAX_SIZE; lba++) {
			ssdAPI->read(lba);
		}
	};

	void fullwrite(string data) {
		for (int lba = 0; lba < 100; lba++) {
			ssdAPI->write(lba, data);
		}
	}
};


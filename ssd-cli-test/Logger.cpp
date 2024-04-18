#include <string>
#include <iostream>
using namespace std;

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}

	void log(string logMsg)
	{
		cout << "[currectTime] FunctionName() :" << logMsg << endl;
	}

private:
	Logger() {

	}

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

};
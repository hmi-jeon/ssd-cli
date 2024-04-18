#include <string>
#include <iostream>
using namespace std;

enum LoggerMode {
	SHELL_MODE,
	RUNNER_MODE
};

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}

	void setLoggerMode(LoggerMode mode) {
		this->mode = mode;
	}

	void print(string logMsg)
	{
		cout << "[currectTime] FunctionName() :" << logMsg << endl;
	}

private:
	Logger() {

	}
	LoggerMode mode = SHELL_MODE;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

};
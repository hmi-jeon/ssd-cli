#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#define  print(str) log(__FUNCTION__,str)

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

	void saveLogger(string logBuffer)
	{

	}

	string makeDateString()
	{
		std::time_t now = std::time(nullptr);
		std::tm* current_time = std::localtime(&now);
		stringstream ss{};
		ss << "[" << std::put_time(current_time, "%y.%m.%d %H:%M") << "]";
		return ss.str();
	}

	bool makeLog(string& logBuffer, string functionName, string logMsg)
	{
		logBuffer = makeDateString() + functionName + ":" + logMsg;
		return true;
	}

	void log(string functionName, string logMsg)
	{
		string logBuffer;
		if (makeLog(logBuffer, functionName, logMsg) == false) {
			return;
		}
		saveLogger(logBuffer);
		if (mode != RUNNER_MODE)
			cout << logBuffer << endl;
	}


private:
	Logger() {

	}
	LoggerMode mode = SHELL_MODE;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	fstream log_fs;

};
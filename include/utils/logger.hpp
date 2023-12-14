#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP

#include <string>
#include <iostream>

namespace util {
class Logger {
public:
	static void SetDebugActive();
	static void Log(const std::string& message, 
		const char* callerFun = __builtin_FUNCTION());
	static void LogDebug(const std::string& message,
		const char* callerFun = __builtin_FUNCTION());
	static void LogWarning(const std::string& message,
		const char* callerFile = __builtin_FILE(),
		const char* callerFun = __builtin_FUNCTION(),
		const int callerLine = __builtin_LINE());
	static void LogError(const std::string& message,
		const char* callerFile = __builtin_FILE(),
		const char* callerFun = __builtin_FUNCTION(),
		const int callerLine = __builtin_LINE());

private:
	static bool debug;
	static void print(const char* caller, const std::string &logType, 
		const std::string &message);
	static void print(const char* callerFile, const int callerLine,
		const std::string& logType, const std::string& message);
};


}

#endif
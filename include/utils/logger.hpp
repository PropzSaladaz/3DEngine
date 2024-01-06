#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP

#include <string>
#include <iostream>

namespace util {
class Logger {
public:
	static void SetDebugActive();
	static bool Log(const std::string& message,
		const char* callerFun = __builtin_FUNCTION());
	static bool LogDebug(const std::string& message,
		const char* callerFun = __builtin_FUNCTION());
	static bool LogWarning(const std::string& message,
		const char* callerFile = __builtin_FILE(),
		const char* callerFun = __builtin_FUNCTION(),
		const int callerLine = __builtin_LINE());
	static bool LogError(const std::string& message,
		const char* callerFile = __builtin_FILE(),
		const char* callerFun = __builtin_FUNCTION(),
		const int callerLine = __builtin_LINE());

private:
	static bool debug;
	static bool print(const char* caller, const std::string &logType,
		const std::string &message);
	static bool print(const char* callerFile, const int callerLine,
		const std::string& logType, const std::string& message);
};


}

#endif
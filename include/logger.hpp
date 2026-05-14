#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stack>
#include <chrono>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <filesystem>
#include <sys/stat.h>
#include <deque>
#include <vector>
#include <optional>
#include <initializer_list>
#include <fstream>
#include "config.hpp"


#ifdef EMULATE
    #define LOG_PATH "log/"
#else
    #define LOG_PATH "log/"
#endif
static constexpr size_t MAX_LINES = 40;
enum class LogLevel { DEBUG, INFO, WARNING, ERROR, GREENINFO};


class ScopeLogger {
private:
    static std::stack<std::string> functionName_;

public:
    ScopeLogger(const std::string& functionName){
        functionName_.push(functionName);
    }

    ~ScopeLogger() {
        functionName_.pop();
    }

    static std::string logIndentation(){
        std::ostringstream returnstring;
        if (!functionName_.empty()) {
            returnstring << "   "<< std::setw(15) <<  std::left << functionName_.top();
        }
        for (size_t i = 1; i < functionName_.size(); ++i) {
            returnstring << "=";
        }
        if (!functionName_.empty()) {
            returnstring << ">";
        }
        return returnstring.str();
    }

};

inline std::stack<std::string> ScopeLogger::functionName_;



class Logger {
private:
    unsigned long m_matchTime = 0;
    int m_x = 0;
    int m_y = 0;
    int m_theta = 0;
    bool logError = false;
    int fileDescriptor = -1;
    std::deque<std::string> lines;

private:
    std::string getPosition(void);
    std::string getTime(void);
    int getLogFileDescriptor();
    void logToFile(const std::string& message, bool forceSync = false);
    std::string getExecutablePath();
    std::string resolveLogPath(const std::string& path);
    int getLogNumber();
    static int globalLogNum;
    int readAndIncrementLogNum(const std::string& fullPath);
    void ensureLogDirectoryAndNumFile(const std::string& logDir, const std::string& numLogFile);
    void logToOutput(const std::string& message);
    void addLine(const std::string& line);

public:
    void initLog(void);

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    template<typename... Args>
    void log(LogLevel level, const std::string& message, Args... args) {
        std::ostringstream oss;
        appendMessage(oss, message, args...);
        std::ostringstream formatted;
        switch (level) {
            case LogLevel::DEBUG:
                formatted << "[DEBUG]" << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation() << " " << oss.str() << std::endl;
                break;
            case LogLevel::INFO:
                formatted << "[INFO] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " " << oss.str() << std::endl;
                break;
            case LogLevel::WARNING:
                formatted << "\033[33m";
                formatted << "[WARNING] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                formatted << "\033[0m";
                break;
            case LogLevel::ERROR:
                logError = true;
                formatted << "\033[1;31m";
                formatted << "[ERROR] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                formatted << "\033[0m";
                break;
            case LogLevel::GREENINFO:
                formatted << "\033[32m";
                formatted << "[INFO] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                formatted << "\033[0m";
                break;

            default:
                break;
        }
        logToOutput(formatted.str());
        logToFile(formatted.str());
    }
    void setTime(unsigned long time);
    bool getLogError();
    void setCoord(int x, int y, int theta);

private:
    Logger() {}

    template<typename T, typename... Args>
    void appendMessage(std::ostringstream& oss, const T& value, Args... args) {
        oss << value;
        appendMessage(oss, args...);
    }

    void appendMessage(std::ostringstream& oss) {}  // Base case for recursion
};





// Macros for easier logging
#define LOG_GET_ERROR() Logger::getInstance().getLogError()
#define LOG_SET_TIME(time) Logger::getInstance().setTime(time)
#define LOG_SET_COORD(x, y, theta) Logger::getInstance().setCoord(x, y, theta)
#define LOG_DEBUG(message, ...) Logger::getInstance().log(LogLevel::DEBUG, message, ##__VA_ARGS__)
#define LOG_INFO(message, ...) Logger::getInstance().log(LogLevel::INFO, message, ##__VA_ARGS__)
#define LOG_GREEN_INFO(message, ...) Logger::getInstance().log(LogLevel::GREENINFO, message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...) Logger::getInstance().log(LogLevel::WARNING, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) Logger::getInstance().log(LogLevel::ERROR, message, ##__VA_ARGS__)
#define LOG_INIT() Logger::getInstance().initLog()
#define LOG_SCOPE(message) ScopeLogger __logger__(message)
#define LOG_STATE(message, ...) Logger::getInstance().log(LogLevel::INFO, message, ##__VA_ARGS__)

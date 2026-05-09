#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stack>

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

private:
    std::string getPosition(void){
        std::ostringstream returnstring;
        returnstring << "[" << std::setw(5) << m_x << " " << std::setw(5) << m_y << " " << std::setw(4) << m_theta <<"]";
        return returnstring.str();
    }

    std::string getTime(void){
        std::ostringstream returnstring;
        returnstring << "[" << std::setw(4) << m_matchTime <<"]";
        return returnstring.str();
    }
public:
    void initLog(void){
        std::cout << "\033[1;31m";
        std::cout << "  /$$$$$$  /$$                                     /$$                              " << std::endl;
        std::cout << " /$$__  $$| $$                                    |__/                              " << std::endl;
        std::cout << "| $$  \\__/| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$   /$$$$$$$" << std::endl;
        std::cout << "| $$      | $$__  $$ /$$__  $$ /$$__  $$ |____  $$| $$| $$__  $$ /$$__  $$ /$$_____/" << std::endl;
        std::cout << "| $$      | $$  \\ $$| $$  \\ $$| $$  \\ $$  /$$$$$$$| $$| $$  \\ $$| $$  \\ $$|  $$$$$$ " << std::endl;
        std::cout << "| $$    $$| $$  | $$| $$  | $$| $$  | $$ /$$__  $$| $$| $$  | $$| $$  | $$ \\____  $$" << std::endl;
        std::cout << "|  $$$$$$/| $$  | $$|  $$$$$$/| $$$$$$$/|  $$$$$$$| $$| $$  | $$|  $$$$$$$ /$$$$$$$/" << std::endl;
        std::cout << " \\______/ |__/  |__/ \\______/ | $$____/  \\_______/|__/|__/  |__/ \\____  $$|_______/ " << std::endl;
        std::cout << "                              | $$                               /$$  \\ $$          " << std::endl;
        std::cout << "                              | $$                              |  $$$$$$/          " << std::endl;
        std::cout << "                              |__/                               \\______/           " << std::endl;
        std::cout << "\033[0m";

        std::cout << "LesChopiangs" << std::endl;
        std::cout << "PROGRAM ROBOT CDFR 2026" << std::endl;
        time_t temps;
        struct tm date;
        char tempsFormate[80];
        time(&temps);
        date = *localtime(&temps);
        strftime(tempsFormate, sizeof(tempsFormate), "%Y-%m-%d %H:%M:%S", &date);
        std::cout << "Start Time : " << tempsFormate << std::endl;
    }

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    template<typename... Args>
    void log(LogLevel level, const std::string& message, Args... args) {
        std::ostringstream oss;
        appendMessage(oss, message, args...);

        switch (level) {
            case LogLevel::DEBUG:
                std::cout << "[DEBUG]" << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation() << " " << oss.str() << std::endl;
                break;
            case LogLevel::INFO:
                std::cout << "[INFO] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " " << oss.str() << std::endl;
                break;
            case LogLevel::WARNING:
                std::cout << "\033[33m";
                std::cout << "[WARNING] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                std::cout << "\033[0m";
                break;
            case LogLevel::ERROR:
                std::cout << "\033[1;31m";
                std::cout << "[ERROR] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                std::cout << "\033[0m";
                break;
            case LogLevel::GREENINFO:
                std::cout << "\033[32m";
                std::cout << "[INFO] " << getTime() << getPosition() << std::setw(25)  <<  std::left << ScopeLogger::logIndentation()  << " "  << oss.str() << std::endl;
                std::cout << "\033[0m";
                break;

            default:
                break;
        }
    }

    void setTime(unsigned long time){
        m_matchTime = time;
    }


    void setCoord(int x, int y, int theta){
        m_x = x;
        m_y = y;
        m_theta = theta;
    }

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

#include "logger.hpp"

int Logger::globalLogNum = -1;

std::string Logger::getPosition(void){
    std::ostringstream returnstring;
    returnstring << "[" << std::setw(5) << m_x << " " << std::setw(5) << m_y << " " << std::setw(4) << m_theta <<"]";
    return returnstring.str();
}

std::string Logger::getTime(void){
    std::ostringstream returnstring;
    returnstring << "[" << std::setw(4) << m_matchTime <<"]";
    return returnstring.str();
}

void Logger::initLog(void){
    std::ostringstream intiMessage;
    intiMessage << "\033[1;31m";
    intiMessage << "  /$$$$$$  /$$                                     /$$                              " << std::endl;
    intiMessage << " /$$__  $$| $$                                    |__/                              " << std::endl;
    intiMessage << "| $$  \\__/| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$   /$$$$$$$" << std::endl;
    intiMessage << "| $$      | $$__  $$ /$$__  $$ /$$__  $$ |____  $$| $$| $$__  $$ /$$__  $$ /$$_____/" << std::endl;
    intiMessage << "| $$      | $$  \\ $$| $$  \\ $$| $$  \\ $$  /$$$$$$$| $$| $$  \\ $$| $$  \\ $$|  $$$$$$ " << std::endl;
    intiMessage << "| $$    $$| $$  | $$| $$  | $$| $$  | $$ /$$__  $$| $$| $$  | $$| $$  | $$ \\____  $$" << std::endl;
    intiMessage << "|  $$$$$$/| $$  | $$|  $$$$$$/| $$$$$$$/|  $$$$$$$| $$| $$  | $$|  $$$$$$$ /$$$$$$$/" << std::endl;
    intiMessage << " \\______/ |__/  |__/ \\______/ | $$____/  \\_______/|__/|__/  |__/ \\____  $$|_______/ " << std::endl;
    intiMessage << "                              | $$                               /$$  \\ $$          " << std::endl;
    intiMessage << "                              | $$                              |  $$$$$$/          " << std::endl;
    intiMessage << "                              |__/                               \\______/           " << std::endl;
    intiMessage << "\033[0m";

    intiMessage << "LesChopiangs" << std::endl;
    intiMessage << "PROGRAM ROBOT CDFR 2026" << std::endl;
    time_t temps;
    struct tm date;
    char tempsFormate[80];
    time(&temps);
    date = *localtime(&temps);
    strftime(tempsFormate, sizeof(tempsFormate), "%Y-%m-%d %H:%M:%S", &date);
    intiMessage << "Start Time : " << tempsFormate << std::endl;
    logToOutput(intiMessage.str());
    logToFile(intiMessage.str());
}


void Logger::setTime(unsigned long time){
    m_matchTime = time;
}

bool Logger::getLogError(){
    return logError;
}


void Logger::setCoord(int x, int y, int theta){
    m_x = x;
    m_y = y;
    m_theta = theta;
}

void Logger::logToFile(const std::string& message, bool forceSync){
    int fd = getLogFileDescriptor();
    if (fd == -1) return;

    std::string fullMessage = message;
    write(fd, fullMessage.c_str(), fullMessage.size());

    if (forceSync) {
        fsync(fd);
    }
}

int Logger::getLogFileDescriptor() {
    if(fileDescriptor != -1)
        return fileDescriptor;

    std::ostringstream oss;
    oss << resolveLogPath(LOG_PATH) << "log_" << getLogNumber() << ".log";
    std::string fullName = oss.str();

    fileDescriptor = open(fullName.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fileDescriptor == -1) {
        perror(("Failed to open log file: " + fullName).c_str());
        return -1;
    }

    return fileDescriptor;
}


std::string Logger::getExecutablePath(){
    char result[1000];
    ssize_t count = readlink("/proc/self/exe", result, sizeof(result));
    if (count == -1) {
        throw std::runtime_error("Unable to determine executable path");
    }
    return std::filesystem::path(std::string(result, count)).parent_path().string();
}

std::string Logger::resolveLogPath(const std::string& path) {
    std::filesystem::path p(path);
    if (p.is_absolute()) {
        return path;
    }
    return (getExecutablePath() / p).string();
}


int Logger::getLogNumber(){
    if (globalLogNum == -1) {
        globalLogNum = readAndIncrementLogNum(resolveLogPath(LOG_PATH));
    }
    return globalLogNum;
}

int Logger::readAndIncrementLogNum(const std::string& fullPath) {
    std::string numFilePath = fullPath+"/numLog";
    ensureLogDirectoryAndNumFile(fullPath,numFilePath);
    std::ifstream in(numFilePath);
    int num = 0;
    if (in >> num) {
        in.close();
    }

    std::ofstream out(numFilePath, std::ios::trunc);
    out << (num + 1);
    return num;
}


void Logger::ensureLogDirectoryAndNumFile(const std::string& logDir, const std::string& numLogFile) {
    struct stat st;
    if (stat(logDir.c_str(), &st) != 0) {
        if (mkdir(logDir.c_str(), 0755) != 0) {
            perror("Failed to create log directory");
            exit(EXIT_FAILURE);
        }
        std::ofstream out(numLogFile);
        if (!out) {
            std::cerr << "Erreur : impossible de créer le fichier " << numLogFile << std::endl;
            exit(EXIT_FAILURE);
        }
        out << 0;
        out.close();
    } else {
        std::ifstream in(numLogFile);
        if (!in) {
            std::cerr << "Erreur : le dossier '" << logDir << "' existe mais pas le fichier 'numLog'. Abandon." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void Logger::logToOutput(const std::string& message){
    std::stringstream ss(message);
    std::string line;
    while (std::getline(ss, line)) {
        addLine(line);
    }

    std::cout << message;
    std::cout.flush();
}

void Logger::addLine(const std::string& line) {
    lines.push_back(line);
    if (lines.size() > MAX_LINES) {
        lines.pop_front();
    }
}
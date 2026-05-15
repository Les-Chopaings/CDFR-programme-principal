#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <array>
#include <math.h>
#include "logger.hpp"

struct RGBColor {
    uint8_t r, g, b;
};

class ArduinoSerial {
public:
    ArduinoSerial(const std::string& port = "/dev/ttyUSB0", int baudrate = B115200);
    ~ArduinoSerial();
    std::array<RGBColor, 4> getColors();
    float compareColors(int i, const RGBColor& c1);
    void printColors(uint8_t i);

private:
    std::string portName;
    int baudRate;
    int serialPort;
    std::atomic<bool> running;
    std::thread readThread;
    std::array<RGBColor, 4> lastColors;
    std::mutex valueMutex;
    void setup();
    void readLoop();
    bool parseLine(const std::string& line, std::array<RGBColor, 4>& out);
};
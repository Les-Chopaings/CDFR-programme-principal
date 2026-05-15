#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

class ArduinoSerial {
public:
    ArduinoSerial(const std::string& port = "/dev/ttyUSB0", int baudrate = B115200);
    ~ArduinoSerial();
    int getLastValue();

private:
    std::string portName;
    int baudRate;

    int serialPort;

    std::atomic<bool> running;
    std::thread readThread;

    int lastValue;
    std::mutex valueMutex;

    void setup();
    void readLoop();

};

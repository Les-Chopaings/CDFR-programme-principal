#include "arduinoSerial.hpp"

void ArduinoSerial::setup() {
    serialPort = open(portName.c_str(), O_RDWR);

    if (serialPort < 0) {
        std::cerr << "Erreur ouverture port série\n";
        return;
    }

    termios tty{};

    if (tcgetattr(serialPort, &tty) != 0) {
        std::cerr << "Erreur configuration port\n";
        return;
    }

    cfsetispeed(&tty, baudRate);
    cfsetospeed(&tty, baudRate);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_iflag = 0;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    tcsetattr(serialPort, TCSANOW, &tty);

    running = true;

    // Lance le thread de lecture
    readThread = std::thread(&ArduinoSerial::readLoop, this);

    std::cout << "ArduinoSerial connecté sur " << portName << std::endl;
}

void ArduinoSerial::readLoop() {
    std::string line;
    char c;

    while (running) {
        int n = read(serialPort, &c, 1);

        if (n > 0) {
            if (c == '\n') {
                try {
                    int value = std::stoi(line);

                    {
                        std::lock_guard<std::mutex> lock(valueMutex);
                        lastValue = value;
                    }

                } catch (...) {
                    // Ignore les lignes invalides
                }

                line.clear();
            }
            else if (c != '\r') {
                line += c;
            }
        }
    }
}

ArduinoSerial::ArduinoSerial(const std::string& port, int baudrate)
    : portName(port),
        baudRate(baudrate),
        serialPort(-1),
        running(false),
        lastValue(0)
{
    setup();
}

ArduinoSerial::~ArduinoSerial() {
    running = false;

    if (readThread.joinable()) {
        readThread.join();
    }

    if (serialPort >= 0) {
        close(serialPort);
    }
}

// Retourne la dernière valeur reçue
int ArduinoSerial::getLastValue() {
    std::lock_guard<std::mutex> lock(valueMutex);
    return lastValue;
}
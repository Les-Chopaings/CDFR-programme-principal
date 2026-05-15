#include "arduinoSerial.hpp"
#include <sstream>

ArduinoSerial::ArduinoSerial(const std::string& port, int baudrate)
    : portName(port),
      baudRate(baudrate),
      serialPort(-1),
      running(false),
      lastColors{}
{
    setup();
}

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
    readThread = std::thread(&ArduinoSerial::readLoop, this);
    std::cout << "ArduinoSerial connecté sur " << portName << std::endl;
}

bool ArduinoSerial::parseLine(const std::string& line, std::array<RGBColor, 4>& out) {
    // Format attendu : R0,G0,B0,R1,G1,B1,R2,G2,B2,R3,G3,B3
    std::istringstream ss(line);
    std::string token;
    int values[12];

    for (int i = 0; i < 12; i++) {
        if (!std::getline(ss, token, ',')) return false;
        try {
            int v = std::stoi(token);
            if (v < 0 || v > 255) return false;
            values[i] = v;
        } catch (...) {
            return false;
        }
    }

    for (int i = 0; i < 4; i++) {
        out[i].r = (uint8_t)values[i * 3];
        out[i].g = (uint8_t)values[i * 3 + 1];
        out[i].b = (uint8_t)values[i * 3 + 2];
    }
    return true;
}

void ArduinoSerial::readLoop() {
    std::string line;
    char c;

    while (running) {
        int n = read(serialPort, &c, 1);
        if (n > 0) {
            if (c == '\n') {
                std::array<RGBColor, 4> parsed;
                if (parseLine(line, parsed)) {
                    std::lock_guard<std::mutex> lock(valueMutex);
                    lastColors = parsed;
                }
                line.clear();
            } else if (c != '\r') {
                line += c;
            }
        }
    }
}

ArduinoSerial::~ArduinoSerial() {
    running = false;
    if (readThread.joinable())
        readThread.join();
    if (serialPort >= 0)
        close(serialPort);
}

std::array<RGBColor, 4> ArduinoSerial::getColors() {
    std::lock_guard<std::mutex> lock(valueMutex);
    return lastColors;
}

float ArduinoSerial::compareColors(int i, const RGBColor& c1)
{
    std::array<RGBColor, 4> color = getColors();
    int dr = (int)c1.r - color[i].r;
    int dg = (int)c1.g - color[i].g;
    int db = (int)c1.b - color[i].b;

    float distance = sqrtf((float)(dr*dr + dg*dg + db*db));

    // distance max possible en RGB
    const float maxDistance = 441.67f;

    float similarity = 100.0f * (1.0f - (distance / maxDistance));

    if (similarity < 0) similarity = 0;

    return similarity;
}

void ArduinoSerial::printColors(uint8_t i){
    std::array<RGBColor, 4> color = getColors();

    LOG_DEBUG("I2C #", i);
    LOG_DEBUG("R: ", color[i].r);
    LOG_DEBUG("G: ", color[i].g);
    LOG_DEBUG("B: ", color[i].b);
}
#include "Arduino.hpp"
#include "logger.hpp"

Arduino::Arduino(int slave_address) : I2CDevice (slave_address){
    if (i2cFile == -1) return; // Emulation
    uint8_t version;
    uint8_t message[] = {0};
}

// [0;180]
void Arduino::moveServo(int position) {
    LOG_DEBUG("Move servo to ", position);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteUInt16(&ptr, position);
    I2cSendData(10, message, 2);
}

void Arduino::test(void) {
    LOG_DEBUG("test i2c");
    if (i2cFile == -1) return; // Emulation
    I2cSendData(200, nullptr, 0);
}
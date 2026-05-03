#include "Arduino.hpp"
#include "logger.hpp"

Arduino::Arduino(int slave_address) : I2CDevice (slave_address){
    if (i2cFile == -1) return; // Emulation
}

// [0;180]
void Arduino::servoMove(servo servoNum, int position){
    LOG_DEBUG("Move ",servo_to_string(servoNum)," to ", position);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteUInt16(&ptr, position);
    I2cSendData((int)servoNum, message, 2);
}

void Arduino::controlePompe(pompe pompeNum, bool enable){
    LOG_DEBUG(pompe_to_string(pompeNum)," to ", enable);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteUInt16(&ptr, enable);
    I2cSendData((int)pompeNum, message, 2);
}

void Arduino::servoAllEnable(bool enable){
    LOG_DEBUG("servo enable ", enable);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteUInt16(&ptr, enable);
    I2cSendData(SERVO_ENABLE, message, 2);
}

void Arduino::stepperEnable(bool enable){
    LOG_DEBUG("stepper enable ", enable);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteUInt16(&ptr, enable);
    I2cSendData(STEPPER_ENABLE, message, 2);
}

void Arduino::stepperMove(int position){
    LOG_DEBUG("stepper move to ", position);
    if (i2cFile == -1) return; // Emulation
    uint8_t message [2];
    uint8_t *ptr = message;
    WriteUInt16(&ptr, position);
    I2cSendData(STEPPER_MOVE_TO, message, 2);
}

bool Arduino::readButton(button buttonNum){
    if (i2cFile == -1) return false; // Emulation
    uint8_t message [2];
    I2cReceiveData((int)buttonNum, message, 2);
    return (bool)message[0];
}

void Arduino::test(void) {
    LOG_DEBUG("test i2c");
    if (i2cFile == -1) return; // Emulation
    I2cSendData(200, nullptr, 0);
}
#include "Colorsensor.hpp"
#include "logger.hpp"

Colorsensor::Colorsensor(int slave_addressSensor, int slave_addressMulti){
    // if (i2cFile == -1) return; // Emulation
    i2cMulti = new I2CDevice(slave_addressMulti);
    i2cColorSensor = new I2CDevice(slave_addressSensor);
}

uint16_t Colorsensor::read16(uint8_t reg){
    uint8_t buffer[2] = {(uint8_t)(TCS34725_COMMAND_BIT | reg), 0};
    i2cColorSensor->I2cReceiveData(buffer[2], buffer, 2);
    return (uint16_t(buffer[1]) << 8) | (uint16_t(buffer[0]) & 0xFF);
}

void Colorsensor::tcaSelect(uint8_t i){
    if (i > 7) return;
    i2cMulti->I2cSendData(1<<i, NULL, 0);
}

void Colorsensor::getRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
    *c = read16(TCS34725_CDATAL);
    *r = read16(TCS34725_RDATAL);
    *g = read16(TCS34725_GDATAL);
    *b = read16(TCS34725_BDATAL);
}

void Colorsensor::printColors(uint8_t i){
    uint16_t r, g, b, c;
    tcaSelect(i);
    getRawData(&r, &g, &b, &c);

    LOG_DEBUG("I2C #", i);
    LOG_DEBUG("R: ", r);
    LOG_DEBUG("G: ", g);
    LOG_DEBUG("B: ", b);
    LOG_DEBUG("C: ", c); LOG_DEBUG(" ");
}
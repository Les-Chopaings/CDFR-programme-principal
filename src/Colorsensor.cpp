#include "Colorsensor.hpp"
#include "logger.hpp"

Colorsensor::Colorsensor(int slave_addressSensor, int slave_addressMulti){
    // if (i2cFile == -1) return; // Emulation
    i2cMulti = new I2CDevice(slave_addressMulti);
    i2cColorSensor = new I2CDevice(slave_addressSensor);
    init(0);
    init(1);
    init(2);
    init(3);
}

void Colorsensor::init(uint8_t i)
{
    tcaSelect(i);

    // Power ON
    write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);

    // petit délai obligatoire (~3ms)
    usleep(3000);

    // Enable ADC
    write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

void Colorsensor::write8(uint8_t reg, uint8_t value)
{
    uint8_t data = value;
    i2cColorSensor->I2cSendData(
        TCS34725_COMMAND_BIT | reg,
        &data,
        1,
        1
    );
}

uint16_t Colorsensor::read16(uint8_t reg){
    uint8_t regAddr = (uint8_t)(TCS34725_COMMAND_BIT | reg);
    uint8_t buffer[2];
    i2cColorSensor->I2cReceiveData(regAddr, buffer, 2);
    return (uint16_t(buffer[1]) << 8) | (uint16_t(buffer[0]) & 0xFF);
}

void Colorsensor::tcaSelect(uint8_t i){
    if (i > 7) return;
    i2cMulti->I2cSendData(1<<i, NULL, 0);
}

void Colorsensor::getRawData(uint8_t i, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
    tcaSelect(i);
    *c = read16(TCS34725_CDATAL);
    *r = read16(TCS34725_RDATAL);
    *g = read16(TCS34725_GDATAL);
    *b = read16(TCS34725_BDATAL);
}

void Colorsensor::printColors(uint8_t i){
    uint16_t r, g, b, c;
    getRawData(i, &r, &g, &b, &c);

    LOG_DEBUG("I2C #", i);
    LOG_DEBUG("R: ", r);
    LOG_DEBUG("G: ", g);
    LOG_DEBUG("B: ", b);
    LOG_DEBUG("C: ", c); LOG_DEBUG(" ");
}
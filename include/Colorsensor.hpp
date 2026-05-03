#pragma once

#include <cstdint>
#include <iostream>
#include <unistd.h>
#include "I2CDevice.hpp"
#define DEC 10

#define TCAADDR (0x70)
#define TCS34725_ADDRESS (0x29)     /**< I2C address **/
#define TCS34725_COMMAND_BIT (0x80) /**< Command bit **/

#define TCS34725_ENABLE 0x00
#define TCS34725_ENABLE_PON 0x01
#define TCS34725_ENABLE_AEN 0x02

#define TCS34725_CDATAL (0x14) /**< Clear channel data low byte */
#define TCS34725_CDATAH (0x15) /**< Clear channel data high byte */
#define TCS34725_RDATAL (0x16) /**< Red channel data low byte */
#define TCS34725_RDATAH (0x17) /**< Red channel data high byte */
#define TCS34725_GDATAL (0x18) /**< Green channel data low byte */
#define TCS34725_GDATAH (0x19) /**< Green channel data high byte */
#define TCS34725_BDATAL (0x1A) /**< Blue channel data low byte */
#define TCS34725_BDATAH (0x1B) /**< Blue channel data high byte */

class Colorsensor{
    I2CDevice* i2cColorSensor;
    I2CDevice* i2cMulti;
public:
    Colorsensor(int slave_addressSensor, int slave_addressMulti);
    void write8(uint8_t reg, uint8_t value);
    void init(uint8_t i);
    uint16_t read16(uint8_t reg);
    void tcaSelect(uint8_t i);
    void getRawData(uint8_t i, uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
    void printColors(uint8_t i);
};
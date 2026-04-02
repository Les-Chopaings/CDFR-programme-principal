#pragma once

#include <cstdint>
#include <iostream>
#include "I2CDevice.hpp"

class Arduino : public I2CDevice {
    using I2CDevice::I2CDevice;

   public:
    Arduino(int slave_address);
    void test(void);
    void moveServo(int position);
};
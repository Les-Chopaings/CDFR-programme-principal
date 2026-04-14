#pragma once

#include <cstdint>
#include <iostream>
#include "I2CDevice.hpp"

#define STEPPER_MOVE_TO 20
#define STEPPER_ENABLE 21

enum class servo{
    bascule = 1,
    slider1 = 2,
    slider2 = 3,
    slider3 = 4,
    slider4 = 5,
    rotation1 = 6,
    rotation2 = 7,
    rotation3 = 8,
    rotation4 = 9,
    temp = 10
};
enum class pompe{
    pompe1 = 30,
    pompe2 = 31,
    pompe3 = 32,
    pompe4 = 33
};
enum class button{
    color = 101,
    magnet = 102
};


inline const char* servo_to_string(servo p) {
    switch (p) {
        case servo::rotation1 : return "rotation1";
        case servo::rotation2 : return "rotation2";
        case servo::rotation3 : return "rotation3";
        case servo::rotation4 : return "rotation4";
        case servo::slider1 : return "slider1";
        case servo::slider2 : return "slider2";
        case servo::slider3 : return "slider3";
        case servo::slider4 : return "slider4";
        case servo::bascule : return "bascule";
        case servo::temp : return "temp";
        default:     return "inconnu";
    }
}

inline const char* pompe_to_string(pompe p) {
    switch (p) {
        case pompe::pompe1: return "pompe1";
        case pompe::pompe2: return "pompe2";
        case pompe::pompe3: return "pompe3";
        case pompe::pompe4: return "pompe4";
        default:     return "inconnu";
    }
}

class Arduino : public I2CDevice {
    using I2CDevice::I2CDevice;
public:
    Arduino(int slave_address);
    void test(void);
    void servoMove(servo servoNum, int position);
    void controlePompe(pompe pompeNum, bool enable);
    void stepperEnable(bool enable);
    void stepperMove(int position);
    bool readButton(button buttonNum);
};
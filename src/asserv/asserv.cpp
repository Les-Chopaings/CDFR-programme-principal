#include "asserv/asserv.h"

Asserv::Asserv(int slave_address) : I2CDevice (slave_address){
    if (i2cFile == -1) return; // Emulation
}

#include <signal.h>

#include "lidarAnalize.hpp"
#include "lidar.hpp"
#include "Arduino.hpp"
#include "asserv/asserv.h"

#define SIZEDATALIDAR 10000


bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}
bool ctrl_z_pressed = false;
void ctrlz(int signal) {
    ctrl_z_pressed = true;
}


int main(int argc, char *argv[]) {
    LOG_INIT();

#ifdef EMULATE
    Arduino arduino(-1);
    Asserv asserv(-1);
#else
    Arduino arduino(0x64);
    Asserv asserv(42);
    if(!lidarSetup("/dev/ttyUSB0",460800)){
        LOG_ERROR("cannot find the lidar");
        return -1;
    }
#endif


    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);
    signal(SIGTSTP, ctrlz);

    lidarAnalize_t lidarData[SIZEDATALIDAR];
    bool prev_collide = false;

    while (1) {

        LOG_SCOPE("Main");


        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            int x, y, teta;
            int distance;
            //robotI2C->getCoords(x,y,teta);
            x = 0; y = 0; teta = 0;
            position_t position = {x,y,teta,0};
            rotateLidarData(lidarData, count, -45);
            convertAngularToAxial(lidarData,count,position);
            if(ctrl_z_pressed){
                ctrl_z_pressed = false;
                pixelArtPrint(lidarData,count,50,50,100,position);
            }
            distance = -200;
            int distance_collide = collide(lidarData,count,distance);
            bool collide = distance_collide<0;
            if(collide != prev_collide){
              prev_collide = collide;
              LOG_DEBUG("COLLIDE : ", collide);
            }
        }

        usleep(10000);


        if (ctrl_c_pressed){
            break;
        }
    }

    LOG_DEBUG("PROCESS KILL");
    arduino.controlePompe(pompe::pompe1,0);
    arduino.controlePompe(pompe::pompe2,0);
    arduino.controlePompe(pompe::pompe3,0);
    arduino.controlePompe(pompe::pompe4,0);
    lidarStop();

    return 0;
}
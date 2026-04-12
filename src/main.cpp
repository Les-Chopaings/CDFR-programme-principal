#include <signal.h>

#include "lidarAnalize.hpp"
#include "lidar.hpp"
#include "Arduino.hpp"
#include "asserv/asserv.h"
#include "utils.h"
#include "GlobalState.h"

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

enum class MainState{
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    START,
    RUN,
    RETURNHOME,
    FIN,
    STOP
};

void stopAndRest(Asserv asserv, Arduino arduino){
    asserv.stop();
    asserv.set_coordinates(0,0,0);
    asserv.set_motor_state(false);
    asserv.set_brake_state(true);
    arduino.controlePompe(pompe::pompe1,0);
    arduino.controlePompe(pompe::pompe2,0);
    arduino.controlePompe(pompe::pompe3,0);
    arduino.controlePompe(pompe::pompe4,0);
    arduino.stepperEnable(false);
};


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
    bool prev_collide = false;
    lidarAnalize_t lidarData[SIZEDATALIDAR];
    GlobalState globalState;

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);
    signal(SIGTSTP, ctrlz);
    usleep(100000);
    MainState currentState = MainState::INIT;
    MainState nextState = MainState::INIT;
    bool initStat = true;

    // TEST
    // asserv.go_to_point(1000, 0, 0);

    while (1) {

        LOG_SCOPE("Main");

        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            int16_t x, y, teta;
            int distance;
            asserv.get_coordinates(x, y, teta);
            LOG_DEBUG("xyt : ",x," ",y," ",teta);
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

        switch (currentState) {
            //****************************************************************
            // wait to start the initialisation process
            case MainState::INIT:{
                if(initStat){
                    LOG_STATE("INIT");
                    //TODO
                }
                if(arduino.readButton(button::magnet)){
                    nextState = MainState::INITIALIZE;
                }
                break;
            }
            //****************************************************************
            // initialisation first step : enable all actuator
            case MainState::INITIALIZE:{
                if(initStat){
                    LOG_STATE("INITIALIZE");
                    if(arduino.readButton(button::color)){
                        globalState.robotColor = ColorTeam::YELLOW;
                    }
                    else{
                        globalState.robotColor = ColorTeam::BLUE;
                    }
                    asserv.set_motor_state(true);
                    asserv.set_brake_state(false);
                    asserv.set_coordinates(0,0,0);
                    arduino.stepperEnable(true);
                }
                nextState = MainState::SETHOME;
                break;
            }
            //****************************************************************
            // initialisation second step : set home
            case MainState::SETHOME:{
                if(initStat){
                     LOG_STATE("SETHOME");
                    //TODO
                }
                nextState = MainState::SETHOME;
                break;
            }
            //****************************************************************
            // wait start from magnet
            case MainState::WAITSTART:{
                if(initStat){
                    LOG_STATE("WAITSTART");
                }
                if(!arduino.readButton(button::magnet)){
                    nextState = MainState::START;
                }
                break;
            }
            //****************************************************************
            // enable all system and start timer
            case MainState::START:{
                if(initStat){
                    LOG_STATE("START");
                }
                globalState.startTimestamp = millis();
                nextState = MainState::RUN;
                break;
            }
            //****************************************************************
            // action to play during 100s
            case MainState::RUN:{
                if(initStat) LOG_STATE("RUN");
                bool finish = false;
                // TODO
                if((globalState.startTimestamp + 100000) < millis() && finish){
                    LOG_GREEN_INFO("END BY TIMER");
                    nextState = MainState::FIN;
                }
                break;
            }
            //****************************************************************
            // disable all actuator
            case MainState::FIN:
                if(initStat){
                    LOG_STATE("FIN");
                }
                nextState = MainState::STOP;
                break;
            //****************************************************************
            // END!!
            case MainState::STOP:
                if(initStat){
                    LOG_STATE("STOP");
                }
                break;
            //****************************************************************
            default:
                LOG_STATE("default");
                nextState = MainState::STOP;
                break;
        }

        initStat = false;
        if(currentState != nextState){
            initStat = true;
        }
        currentState = nextState;

        if (ctrl_c_pressed){
            break;
        }

        usleep(10000);
    }

    LOG_DEBUG("PROCESS KILL");
    lidarStop();

    return 0;
}
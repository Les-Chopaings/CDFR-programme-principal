#pragma once

#include "logger.hpp"
#include "utils.h"
#include "Arduino.hpp"
#define TIMESLIDER 500
#define TIMEROTATE 1000
// #define TIMESLIDER 50
// #define TIMEROTATE 50
class Minifsm
{
private:
    /* data */
    int T_start;
    bool toCopy;
    bool finished;
    int servoToMove[4] = {0, 180, 180, 180};
    int servoCurrentPos[4] = {90, 90, 90, 90};
    bool servoToRot[4] = {0, 0, 0, 0};
    bool servoCurrentRot[4] = {0, 0, 0, 0};
    bool regular;
    enum class State{
        rotate1,
        rotate2,
        rotate3,
        rotate4,
        idle,
        toProcess,
        movement,
        wait,
        rotwait,
        init
    };
    State followupState = State::toProcess;
    State currentState = State::idle;


public:
    Minifsm(/* args */);
    ~Minifsm();
    int TriNoisette(bool* rotate, Arduino* arduino);
    int MoveUp();
    int MoveDown();

};


#pragma once

#include "logger.hpp"
#include "utils.h"
#include "Arduino.hpp"
#define TIMESLIDER 5000
#define TIMEROTATE 5000
class Minifsm
{
private:
    /* data */
    int T_start;
    int startSeconds;
    bool toCopy;
    bool toReset;
    int servoToMove[4] = {0, 180, 180, 180};
    int servoCurrentPos[4] = {90, 90, 90, 90};
    bool rotCopied[4] = {0, 0, 0, 0};
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


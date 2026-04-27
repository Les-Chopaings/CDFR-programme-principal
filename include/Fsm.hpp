#pragma once

#include "Arduino.hpp"
#include "asserv/asserv.h"
#include "GlobalState.h"
#include "logger.hpp"

#define TIMESLIDER 500
#define TIMEROTATE 1000

DECLARE_ENUM_CLASS(FsmTakeNuts,
    INIT,
    TAKE_DOWN,
    TAKE_LEFT,
    TAKE_RIGHT,
    TAKE_UP,        //<-- end : robot full
    TAKE_PIVOT_90,
    READ_SENSOR,
    SORT,           // nathan job
    WAIT_PUT,
    PUT_PIVOT_45,
    PUT_STOP_POMPE,
    PUT_PIVOT_90,   //<-- end : robot empty
    RESET_SORT,     // nathan job
    RESET_DOWN      //<-- end : robot ready
)

DECLARE_ENUM_CLASS(FsmTemp,
    INIT,
    DEPLOY,
    FORWARD,
    RETRACT,
    WAIT,
    COLIDE
)


class Fsm
{
private:
    /* tri */
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
    State triCurrentState = State::idle;

    /* data */
    bool rotation[4];
    FsmTakeNuts currentState = FsmTakeNuts::INIT;
    bool initStat = false;
    unsigned long startTime;

public:
    Fsm(/* args */);
    ~Fsm();
    bool takeNutsRun(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
private:
    int TriNoisette(bool* rotate, Arduino* arduino);
};

int pushTemp(GlobalState* globalState, Asserv* asserv, Arduino* arduino);



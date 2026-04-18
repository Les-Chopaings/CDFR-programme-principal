#pragma once

#include "Arduino.hpp"
#include "asserv/asserv.h"
#include "GlobalState.h"
#include "logger.hpp"


DECLARE_ENUM_CLASS(FsmTakeNuts,
    INIT,
    TAKE_DOWN,
    TAKE_LEFT,
    TAKE_RIGHT,
    TAKE_UP,        //<-- end : robot full
    TAKE_PIVOT_90,
    SORT,           // nathan job
    WAIT_PUT,
    PUT_PIVOT_45,
    PUT_STOP_POMPE,
    PUT_PIVOT_90,   //<-- end : robot empty
    RESET_SORT,     // nathan job
    RESET_DOWN      //<-- end : robot ready
)


class Fsm
{
private:
    /* data */
    FsmTakeNuts currentState = FsmTakeNuts::INIT;
    bool initStat = false;
    unsigned long startTime;

public:
    Fsm(/* args */);
    ~Fsm();
    bool takeNutsRun(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
};



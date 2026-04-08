#pragma once

#include "logger.hpp"


class Minifsm
{
private:
    /* data */
    
    int T_start;
    enum class State{
        etape1,
        etape2,
        etape3,
        idle
    };
    State currentState = State::idle;


public:
    Minifsm(/* args */);
    ~Minifsm();
    int TriNoisette(bool* rotate);
    int MoveUp();
    int MoveDown();

};


#pragma once

#include "logger.hpp"
#include "utils.h"


class Minifsm
{
private:
    /* data */
    int T_start;
    int startSeconds;
    bool toCopy;
    bool rotCopied[4] = {0, 0, 0, 0};
    enum class State{
        rotate1,
        rotate2,
        rotate3,
        rotate4,
        idle,
        toProcess};
    State currentState = State::idle;


public:
    Minifsm(/* args */);
    ~Minifsm();
    int TriNoisette(bool* rotate);
    int MoveUp();
    int MoveDown();

};


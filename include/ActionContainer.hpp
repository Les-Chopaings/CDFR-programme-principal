#pragma once

#include <vector>

#include "Action.hpp"
#include "GlobalState.h"
#include "asserv/asserv.h"
#include "Arduino.hpp"


class ActionContainer
{
private:
    Action* takeStock0;
    Action* takeStock0bis;
    Action* takeStock1;
    Action* takeStock2;
    Action* takeStock3;
    Action* takeStock4;
    Action* takeStock5;
    Action* takeStock6;
    Action* takeStock7;
    Action* takeStock7bis;

    std::vector<Action*> listeAction;
    Action* currentAction = nullptr;

public:
    ActionContainer(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    void initAction(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    int actionContainerRun(void);
    ~ActionContainer();
private :

    int choosNextAction(void);
};


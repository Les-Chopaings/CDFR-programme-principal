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
    std::vector<Action*> listeAction;
    Action* currentAction;

public:
    ActionContainer(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    void initAction(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    int actionContainerRun(void);
    ~ActionContainer();
private :

    int choosNextAction(void);
};


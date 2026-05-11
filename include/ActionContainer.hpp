#pragma once

#include <vector>
#include <memory>

#include "Action.hpp"
#include "GlobalState.h"
#include "asserv/asserv.h"
#include "Arduino.hpp"
#include "Fsm.hpp"


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

    Action* temperatureYellow;
    Action* temperatureBlue;

    Action* returnToHomeYellow;
    Action* returnToHomeBleu;

    std::vector<Action*> listeAction;
    Action* currentAction = nullptr;

    GlobalState* m_globalState = nullptr;

public:
    ActionContainer(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    void initAction(GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    int actionContainerRun(void);
    void updateMap(void);
    ~ActionContainer();
private :

    int choosNextAction(void);
};


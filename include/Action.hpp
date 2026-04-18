#pragma once

#include <functional>
#include <string>

#include "logger.hpp"
#include "GlobalState.h"
#include "asserv/asserv.h"
#include "Arduino.hpp"


class Action
{
private:


    enum class FsmAction{
        INIT,
        MOVESTART,
        STARTACTION,
        ACTION,
        MOVEEND
    };
    const char* fsmAction_to_string(FsmAction p) {
        switch (p) {
            case FsmAction::INIT : return "INIT";
            case FsmAction::MOVESTART : return "MOVESTART";
            case FsmAction::ACTION : return "ACTION";
            case FsmAction::MOVEEND : return "MOVEEND";
            default:     return "inconnu";
        }
    }

private:
    GlobalState* mGlobalState;
    Asserv* mAsserv;
    Arduino* mArduino;


    std::function<int(GlobalState*, Asserv*, Arduino*)> runActionPtr;
    std::function<void(GlobalState*, Asserv*, Arduino*)> startActionPtr;
    std::function<int(GlobalState*)> costActionPtr;
    std::function<void(GlobalState*)> goodEndPtr;
    std::function<void(GlobalState*)> badEndPtr;

    position_t startPostion;
    Direction startDirection;
    Rotation startRotation;

    bool noEndPoint = true;
    position_t endPostion;
    Direction endDirection;
    Rotation endRotation;

    FsmAction currentState = FsmAction::INIT;

    std::string actionName;

    bool noTetaStart = false;

public:
    Action(std::string name, GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    int runAction(void);
    void setStartPoint(int x, int y, int teta, Direction Direction, Rotation rotation);
    void setStartPoint(int x, int y, Direction Direction, Rotation rotation);
    void setEndPoint(int x, int y, int teta, Direction Direction, Rotation rotation);
    int costAction(void);
    void setFunctRunAction(std::function<int(GlobalState*, Asserv*, Arduino*)> ptr);
    void setFunctStartAction(std::function<void(GlobalState*, Asserv*, Arduino*)> ptr);
    void setFunctGoodEnd(std::function<void(GlobalState*)> ptr);
    void setFunctBadEnd(std::function<void(GlobalState*)> ptr);
    void setFunctCostAction(std::function<int(GlobalState*)> ptr);
    std::string getName(void);
    ~Action();

    friend std::ostream& operator<<(std::ostream& os, Action& obj) {
        os << obj.getName();
        return os;
    }

private:
    int goToStart(void);
    int goToEnd(void);
};

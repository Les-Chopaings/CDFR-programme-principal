#pragma once

#include <functional>
#include <string>

#include <math.h>
#include "logger.hpp"
#include "GlobalState.h"
#include "asserv/asserv.h"
#include "Arduino.hpp"
#include "path_finding.h"
#include "utils.h"


class Action
{
private:

    DECLARE_ENUM_CLASS(FsmGoToStart,
        INIT,
        WAIT,
        WAIT_COLIDE,
        COLIDE
    )

    DECLARE_ENUM_CLASS(FsmGoToEnd,
        INIT,
        WAIT
    )

    DECLARE_ENUM_CLASS(FsmAction,
        INIT,
        MOVESTART,
        STARTACTION,
        ACTION,
        MOVEEND
    )

private:
    GlobalState* mGlobalState;
    Asserv* mAsserv;
    Arduino* mArduino;
    path_t mpath;


    std::function<int(GlobalState*, Asserv*, Arduino*)> runActionPtr;
    std::function<void(GlobalState*, Asserv*, Arduino*)> startActionPtr;
    std::function<int(GlobalState*)> costActionPtr;
    std::function<void(GlobalState*)> goodEndPtr;
    std::function<void(GlobalState*)> badEndPtr;

    position_t startPostion;
    Direction startDirection;
    Rotation startRotation;

    bool noEndPoint = true;
    bool noEndAngle = true;
    position_t endPostion;
    Direction endDirection;
    Rotation endRotation;
    bool noTetaStart = true;

    FsmAction currentState = FsmAction::INIT;
    FsmGoToStart currentStateToStart = FsmGoToStart::INIT;
    FsmGoToEnd currentGoToStart = FsmGoToEnd::INIT;
    unsigned long mStartColide;

    std::string actionName;

public:
    Action(std::string name, GlobalState* globalState, Asserv* asserv, Arduino* arduino);
    int runAction(void);
    void setStartPoint(int x, int y, int theta, Direction Direction, Rotation rotation);
    void setStartPoint(int x, int y, Direction Direction);
    void setEndPoint(int x, int y, int theta, Direction Direction, Rotation rotation);
    void setEndAngle(int theta, Rotation rotation);
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

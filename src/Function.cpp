#include "Function.hpp"

enum class FsmTakeNuts{
    INIT,
    DOWN,
    LEFT,
    RiGHT,
    UP,
    PIVOT
};
const char* FsmTakeNuts_to_string(FsmTakeNuts p) {
    switch (p) {
        case FsmTakeNuts::INIT : return "INIT";
        case FsmTakeNuts::DOWN : return "DOWN";
        case FsmTakeNuts::LEFT : return "LEFT";
        case FsmTakeNuts::RiGHT : return "RiGHT";
        case FsmTakeNuts::UP : return "UP";
        case FsmTakeNuts::PIVOT : return "PIVOT";
        default:     return "inconnu";
    }
}

bool takeNuts(GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    static FsmTakeNuts currentState = FsmTakeNuts::INIT;
    FsmTakeNuts nextState = currentState;
    static bool initStat = false;
    static unsigned long startTime;
    bool bret = false;

    switch (currentState)
    {
    case FsmTakeNuts::INIT :
        nextState = FsmTakeNuts::DOWN;
        break;
    case FsmTakeNuts::DOWN :
        if(initStat){
            startTime = millis()+200;
            arduino->controlePompe(pompe::pompe1,1);
            arduino->controlePompe(pompe::pompe2,1);
            arduino->controlePompe(pompe::pompe3,1);
            arduino->controlePompe(pompe::pompe4,1);
            arduino->stepperMove(25);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::LEFT;
        }
        break;
    case FsmTakeNuts::LEFT :
        if(initStat){
            startTime = millis()+750;
            arduino->servoMove(servo::rotation1,10);
            arduino->servoMove(servo::rotation2,10);
            arduino->servoMove(servo::rotation3,10);
            arduino->servoMove(servo::rotation4,10);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::RiGHT;
        }
        break;
    case FsmTakeNuts::RiGHT :
        if(initStat){
            startTime = millis()+750;
            arduino->servoMove(servo::rotation1,0);
            arduino->servoMove(servo::rotation2,0);
            arduino->servoMove(servo::rotation3,0);
            arduino->servoMove(servo::rotation4,0);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::UP;
        }
        break;
    case FsmTakeNuts::UP :
        if(initStat){
            startTime = millis()+2000;
            arduino->stepperMove(1150);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::PIVOT;
        }
        break;
    case FsmTakeNuts::PIVOT :
        if(initStat){
            startTime = millis()+2000;
            arduino->servoMove(servo::bascule,180);
        }
        if(startTime < millis()){
            bret = true;
            nextState = FsmTakeNuts::INIT;
        }
        break;
    default:
        nextState = FsmTakeNuts::INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        LOG_STATE(FsmTakeNuts_to_string(nextState));
        initStat = true;
    }
    currentState = nextState;
    return bret;
}
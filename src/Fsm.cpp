#include "Fsm.hpp"

Fsm::Fsm(/* args */)
{
}

Fsm::~Fsm()
{
}


bool Fsm::takeNutsRun(GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    FsmTakeNuts nextState = currentState;
    bool bret = false;

    switch (currentState)
    {
    case FsmTakeNuts::INIT :
        nextState = FsmTakeNuts::TAKE_DOWN;
        break;
    case FsmTakeNuts::TAKE_DOWN :
        if(initStat){
            startTime = millis()+200;
            arduino->controlePompe(pompe::pompe1,1);
            arduino->controlePompe(pompe::pompe2,1);
            arduino->controlePompe(pompe::pompe3,1);
            arduino->controlePompe(pompe::pompe4,1);
            arduino->stepperMove(25);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::TAKE_LEFT;
        }
        break;
    case FsmTakeNuts::TAKE_LEFT :
        if(initStat){
            startTime = millis()+750;
            arduino->servoMove(servo::rotation1,10);
            arduino->servoMove(servo::rotation2,10);
            arduino->servoMove(servo::rotation3,10);
            arduino->servoMove(servo::rotation4,10);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::TAKE_RIGHT;
        }
        break;
    case FsmTakeNuts::TAKE_RIGHT :
        if(initStat){
            startTime = millis()+750;
            arduino->servoMove(servo::rotation1,0);
            arduino->servoMove(servo::rotation2,0);
            arduino->servoMove(servo::rotation3,0);
            arduino->servoMove(servo::rotation4,0);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::TAKE_UP;
        }
        break;
    case FsmTakeNuts::TAKE_UP :
        if(initStat){
            startTime = millis()+2000;
            arduino->stepperMove(1150);
        }
        if(startTime < millis()){
            nextState = FsmTakeNuts::TAKE_PIVOT_90;
        }
        break;
    case FsmTakeNuts::TAKE_PIVOT_90 :
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
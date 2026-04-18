#include "Action.hpp"


Action::Action(std::string name, GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    mGlobalState = globalState;
    mAsserv = asserv;
    mArduino = arduino;
    noEndPoint = true;
    currentState = FsmAction::INIT;

    runActionPtr = nullptr;
    costActionPtr = nullptr;
    goodEndPtr = nullptr;
    badEndPtr = nullptr;
}

Action::~Action(){

}

//*******************************************************
// FSM action
//*******************************************************
int Action::runAction(void){
    LOG_SCOPE("Action");
    int ireturn = 0;
    FsmAction nextState = currentState;
    int deplacementreturn;

    switch (currentState)
    {
    case FsmAction::INIT :
        nextState = FsmAction::MOVESTART;
        break;

    case FsmAction::MOVESTART :
        deplacementreturn = goToStart();
        if(deplacementreturn>0){
            nextState = FsmAction::ACTION;
        }
        else if(deplacementreturn<0){
            nextState = FsmAction::INIT;
            if(badEndPtr){
                badEndPtr(mGlobalState);
            }
            ireturn = -1;
        }
        break;

    case FsmAction::STARTACTION :
        if(startActionPtr){
            startActionPtr(mGlobalState,mAsserv,mArduino);
        }

    case FsmAction::ACTION :
        deplacementreturn = runActionPtr(mGlobalState,mAsserv,mArduino);
        if(deplacementreturn>0){
            if(noEndPoint){
                nextState = FsmAction::INIT;
                ireturn = 1;
            }
            else{
                nextState = FsmAction::MOVEEND;
            }
            if(goodEndPtr){
                goodEndPtr(mGlobalState);
            }
        }
        else if(deplacementreturn<0){
            nextState = FsmAction::INIT;
            if(badEndPtr){
                badEndPtr(mGlobalState);
            }
            ireturn = -1;
        }
        break;


    case FsmAction::MOVEEND :
        deplacementreturn = goToEnd();
        if(deplacementreturn>0){
            nextState = FsmAction::INIT;
            ireturn = 1;
        }
        else if(deplacementreturn<0){
            nextState = FsmAction::INIT;
            if(badEndPtr){
                badEndPtr(mGlobalState);
            }
            ireturn = -1;
        }
        break;


    default:

        nextState = FsmAction::INIT;
        break;
    }

    if(nextState != currentState){
        LOG_STATE(fsmAction_to_string(nextState));
    }
    currentState = nextState;
    return ireturn;
}

//*******************************************************
// set Function
//*******************************************************
void Action::setFunctCostAction(std::function<int(GlobalState*)> ptr){
    costActionPtr = ptr;
}

void Action::setFunctRunAction(std::function<int(GlobalState*, Asserv*, Arduino*)> ptr){
    runActionPtr = ptr;
}

void Action::setFunctStartAction(std::function<void(GlobalState*, Asserv*, Arduino*)> ptr){
    startActionPtr = ptr;
}

void Action::setFunctGoodEnd(std::function<void(GlobalState*)> ptr){
    goodEndPtr = ptr;
}
void Action::setFunctBadEnd(std::function<void(GlobalState*)> ptr){
    badEndPtr = ptr;
}

void Action::setStartPoint(int x, int y, int teta, Direction Direction, Rotation rotation){
    startPostion.x = x;
    startPostion.y = y;
    startPostion.teta = teta;
    startDirection = Direction;
    startRotation = rotation;
}

void Action::setStartPoint(int x, int y, Direction Direction, Rotation rotation){
    startPostion.x = x;
    startPostion.y = y;
    startDirection = Direction;
    startRotation = rotation;
    noTetaStart = true;
}

void Action::setEndPoint(int x, int y, int teta, Direction Direction, Rotation rotation){
    endPostion.x = x;
    endPostion.y = y;
    endPostion.teta = teta;
    endDirection = Direction;
    endRotation = rotation;
    noEndPoint = false;
}

//*******************************************************
// execute Function
//*******************************************************
int Action::costAction(void){
    return costActionPtr(mGlobalState);
}


int Action::goToStart(void){
    // TODO
    return 0;
}


int Action::goToEnd(void){
    //TODO
    return 0;
}

std::string Action::getName(void){
    return actionName;
}

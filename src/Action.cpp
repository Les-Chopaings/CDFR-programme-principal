#include "Action.hpp"


Action::Action(std::string name, GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    mGlobalState = globalState;
    mAsserv = asserv;
    mArduino = arduino;
    noEndPoint = true;
    currentState = FsmAction::INIT;
    actionName = name;

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
            nextState = FsmAction::STARTACTION;
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
        nextState = FsmAction::ACTION;

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

void Action::setStartPoint(int x, int y, int theta, Direction Direction, Rotation rotation){
    startPostion.x = x;
    startPostion.y = y;
    startPostion.theta = theta;
    startDirection = Direction;
    startRotation = rotation;
    noTetaStart = false;
}

void Action::setStartPoint(int x, int y, Direction Direction){
    startPostion.x = x;
    startPostion.y = y;
    startDirection = Direction;
    noTetaStart = true;
}

void Action::setEndPoint(int x, int y, int theta, Direction Direction, Rotation rotation){
    endPostion.x = x;
    endPostion.y = y;
    endPostion.theta = theta;
    endDirection = Direction;
    endRotation = rotation;
    noEndPoint = false;
}

//*******************************************************
// execute Function
//*******************************************************
int Action::costAction(void){
    int actionCost = costActionPtr(mGlobalState);
    if(actionCost > 0){
        path_t tmpPath = mGlobalState->map.find_path_between_points(
            {(float)mGlobalState->robotPosition.x, (float)mGlobalState->robotPosition.y, 255, 0},
            {(float)startPostion.x, (float)startPostion.y, 255, -M_PI}
        );
        actionCost = (actionCost*10000) / tmpPath.length;
        actionCost = clip(actionCost,0,INT32_MAX);
    }
    return actionCost;
}


int Action::goToStart(void){
    LOG_SCOPE("Action");
    FsmGoToStart nextState = currentStateToStart;
    int deplacementreturn = 0;

    switch (currentStateToStart)
    {
    case FsmGoToStart::INIT :
        mpath = mGlobalState->map.find_path_between_points(
            {(float)mGlobalState->robotPosition.x, (float)mGlobalState->robotPosition.y, 255, 0},
            {(float)startPostion.x, (float)startPostion.y, 255, -M_PI}
        );
        mGlobalState->map.print_Path(mpath);
        if(mpath.length < 2){
            LOG_WARNING("Path not find");
            deplacementreturn = -1;
        }
        else{
            for (size_t i = 0; i < mpath.v.size(); ++i) {
                auto [point, node_id, theta] = mpath.v[i];

                if (i == 0){
                    //Nothing
                }
                else if(i == mpath.v.size()-1){
                    if(noTetaStart)
                        mAsserv->go_to_point(point.x,point.y,Rotation::SHORTEST,startDirection);
                    else
                        mAsserv->go_to_point(point.x,point.y,tetaEnd,Rotation::SHORTEST,startDirection,startRotation);
                }
                else{
                    mAsserv->go_to_point(point.x,point.y);
                }
            }
        }
        nextState = FsmGoToStart::WAIT;
        break;

    case FsmGoToStart::WAIT :
        if(mGlobalState->collideDistance<0){
            nextState = FsmGoToStart::COLIDE;
            mStartColide = millis() + 5000;
            mAsserv->pause();
        }
        else if(mAsserv->get_moving_is_done() && mAsserv->get_command_buffer_size() == 0){
            nextState = FsmGoToStart::INIT;
            deplacementreturn = 1;
        }
        break;

    case FsmGoToStart::COLIDE :
        if(mGlobalState->collideDistance>50){
            nextState = FsmGoToStart::WAIT;
            mAsserv->resume();
        }
        else if(mStartColide<millis()){
            nextState = FsmGoToStart::INIT;
            mAsserv->stop();
            deplacementreturn = -1;
        }
        break;

    default:

        nextState = FsmGoToStart::INIT;
        break;
    }

    if(nextState != currentStateToStart){
        LOG_STATE(FsmGoToStart_to_string(nextState));
    }
    currentStateToStart = nextState;
    return deplacementreturn;
}


int Action::goToEnd(void){
    //TODO
    return 0;
}

std::string Action::getName(void){
    return actionName;
}

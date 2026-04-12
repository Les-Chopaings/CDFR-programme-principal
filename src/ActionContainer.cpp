#include "ActionContainer.hpp"
#include "logger.hpp"

ActionContainer::ActionContainer(GlobalState* globalState, Asserv* asserv, Arduino* arduino){

    takeStock0 = new Action("takeStock0",globalState,asserv,arduino);

}
void ActionContainer::initAction(GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    // TAKE_STOCK_0
    takeStock0->setStartPoint(0,0,0,Direction::SHORTEST,Rotation::SHORTEST);
    takeStock0->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        // TODO
        // return takeStock(*iRobot,iAsser,iarduino,itable,itable->plantPosition[0].y,itable->plantPosition[0].x - MARGESTOCKPLANT,itable->plantPosition[0].x + 400,0);
        return 1;
    });
    takeStock0->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[0] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock0->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[0]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    listeAction.push_back(takeStock0);

    //Choose first Action
    choosNextAction();
}

int ActionContainer::actionContainerRun(void){
    int iActionReturn;
    int iChoosNextReturn = 0;
    int iRet = 0;
    iActionReturn = currentAction->runAction();
    if(iActionReturn == -100){
        iRet = -100;
    }
    else if(iActionReturn!=0){
        iChoosNextReturn = choosNextAction();
    }

    if(iChoosNextReturn == -1){
        iRet = -1;
    }
    return iRet;
}

ActionContainer::~ActionContainer(){
}


int ActionContainer::choosNextAction(void){
    LOG_GREEN_INFO("CHOOSE NEW ACTION: ");
    int bestCost = -1;
    for (Action* elem : listeAction) {
        int cost = elem->costAction();
        if(cost>bestCost){
            bestCost = cost;
            currentAction = elem;
        }
    }
    if(bestCost!=-1){
        LOG_GREEN_INFO("CHOOSE : ",currentAction->getName());
    }
    else{
        LOG_WARNING("NO ACTION CHOOSE");
    }
    return bestCost;
}
#include "ActionContainer.hpp"
#include "logger.hpp"

ActionContainer::ActionContainer(GlobalState* globalState, Asserv* asserv, Arduino* arduino){

    takeStock0 = new Action("takeStock0",globalState,asserv,arduino);

}
void ActionContainer::initAction(GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    // TAKE_STOCK_0

    takeStock0->setStartPoint    (xZoneStock[0],yZoneStock[0]+OFFSET_STOCK,90, Direction::FORWARD,Rotation::SHORTEST);
    takeStock0bis->setStartPoint (xZoneStock[0],yZoneStock[0]-OFFSET_STOCK,-90,Direction::FORWARD,Rotation::SHORTEST);
    takeStock1->setStartPoint    (xZoneStock[1]-OFFSET_STOCK,yZoneStock[1],0,  Direction::FORWARD,Rotation::SHORTEST);
    takeStock2->setStartPoint    (xZoneStock[2]-OFFSET_STOCK,yZoneStock[2],0,  Direction::FORWARD,Rotation::SHORTEST);
    takeStock3->setStartPoint    (xZoneStock[3],yZoneStock[3]+OFFSET_STOCK,90, Direction::FORWARD,Rotation::SHORTEST);
    takeStock4->setStartPoint    (xZoneStock[4],yZoneStock[4]+OFFSET_STOCK,90, Direction::FORWARD,Rotation::SHORTEST);
    takeStock5->setStartPoint    (xZoneStock[5]+OFFSET_STOCK,yZoneStock[5],180,Direction::FORWARD,Rotation::SHORTEST);
    takeStock6->setStartPoint    (xZoneStock[6]+OFFSET_STOCK,yZoneStock[6],180,Direction::FORWARD,Rotation::SHORTEST);
    takeStock7->setStartPoint    (xZoneStock[7],yZoneStock[7]+OFFSET_STOCK,90, Direction::FORWARD,Rotation::SHORTEST);
    takeStock7bis->setStartPoint (xZoneStock[7],yZoneStock[7]-OFFSET_STOCK,-90,Direction::FORWARD,Rotation::SHORTEST);

    takeStock0->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[0] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock0bis->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[0] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock1->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[1] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock2->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[2] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock3->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[3] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock4->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[4] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock5->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[5] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock6->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[6] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock7->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[7] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });
    takeStock7bis->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[7] = ControlOwner::Friendly;
        globalState->robotFull = true;
    });

    takeStock0->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[0]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock0bis->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[0]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock1->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[1]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock2->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[2]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock3->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[3]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock4->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[4]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock5->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[5]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock6->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[6]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock7->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[7]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });
    takeStock7bis->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[7]==ControlOwner::None && globalState->robotFull==false ? 1 : -1;
    });

    takeStock0->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock0bis->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock1->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock2->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock3->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock4->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock5->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock6->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock7->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });
    takeStock7bis->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        return 1;
    });

    // takeStock0->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
    //     // TODO
    //     // return takeStock(*iRobot,iAsser,iarduino,itable,itable->plantPosition[0].y,itable->plantPosition[0].x - MARGESTOCKPLANT,itable->plantPosition[0].x + 400,0);
    //     return 0;
    // });
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
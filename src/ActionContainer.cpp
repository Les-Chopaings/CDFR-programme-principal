#include "ActionContainer.hpp"
#include "logger.hpp"
#include "map/map.h"

ActionContainer::ActionContainer(GlobalState* globalState, Asserv* asserv, Arduino* arduino){

    takeStock0    = new Action("takeStock0",globalState,asserv,arduino);
    takeStock0bis = new Action("takeStock0bis",globalState,asserv,arduino);
    takeStock1    = new Action("takeStock1",globalState,asserv,arduino);
    takeStock2    = new Action("takeStock2",globalState,asserv,arduino);
    takeStock3    = new Action("takeStock3",globalState,asserv,arduino);
    takeStock4    = new Action("takeStock4",globalState,asserv,arduino);
    takeStock5    = new Action("takeStock5",globalState,asserv,arduino);
    takeStock6    = new Action("takeStock6",globalState,asserv,arduino);
    takeStock7    = new Action("takeStock7",globalState,asserv,arduino);
    takeStock7bis = new Action("takeStock7bis",globalState,asserv,arduino);
    temperatureYellow = new Action("temperatureYellow",globalState,asserv,arduino);
    temperatureBlue = new Action("temperatureBlue",globalState,asserv,arduino);
    currentAction = nullptr;
    initAction(globalState, asserv, arduino);

}
void ActionContainer::initAction(GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    // TAKE_STOCK_0

    takeStock0->setStartPoint    (STOCK_X_P0_0, STOCK_Y_P0_0, STOCK_T_P0_0, Direction::FORWARD,Rotation::SHORTEST);
    takeStock0bis->setStartPoint (STOCK_X_P1_0, STOCK_Y_P1_0, STOCK_T_P1_0, Direction::FORWARD,Rotation::SHORTEST);
    takeStock1->setStartPoint    (STOCK_X_P0_1, STOCK_Y_P0_1, STOCK_T_P0_1, Direction::FORWARD,Rotation::SHORTEST);
    takeStock2->setStartPoint    (STOCK_X_P0_2, STOCK_Y_P0_2, STOCK_T_P0_2, Direction::FORWARD,Rotation::SHORTEST);
    takeStock3->setStartPoint    (STOCK_X_P0_3, STOCK_Y_P0_3, STOCK_T_P0_3, Direction::FORWARD,Rotation::SHORTEST);
    takeStock4->setStartPoint    (STOCK_X_P0_4, STOCK_Y_P0_4, STOCK_T_P0_4, Direction::FORWARD,Rotation::SHORTEST);
    takeStock5->setStartPoint    (STOCK_X_P0_5, STOCK_Y_P0_5, STOCK_T_P0_5, Direction::FORWARD,Rotation::SHORTEST);
    takeStock6->setStartPoint    (STOCK_X_P0_6, STOCK_Y_P0_6, STOCK_T_P0_6, Direction::FORWARD,Rotation::SHORTEST);
    takeStock7->setStartPoint    (STOCK_X_P0_7, STOCK_Y_P0_7, STOCK_T_P0_7, Direction::FORWARD,Rotation::SHORTEST);
    takeStock7bis->setStartPoint (STOCK_X_P1_7, STOCK_Y_P1_7, STOCK_T_P1_7, Direction::FORWARD,Rotation::SHORTEST);

    takeStock0->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[0] = ControlOwner::Friendly;
    });
    takeStock0bis->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[0] = ControlOwner::Friendly;
    });
    takeStock1->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[1] = ControlOwner::Friendly;
    });
    takeStock2->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[2] = ControlOwner::Friendly;
    });
    takeStock3->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[3] = ControlOwner::Friendly;
    });
    takeStock4->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[4] = ControlOwner::Friendly;
    });
    takeStock5->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[5] = ControlOwner::Friendly;
    });
    takeStock6->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[6] = ControlOwner::Friendly;
    });
    takeStock7->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[7] = ControlOwner::Friendly;
    });
    takeStock7bis->setFunctGoodEnd([](GlobalState* globalState){
        globalState->zoneStock[7] = ControlOwner::Friendly;
    });

    takeStock0->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[0]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock0bis->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[0]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock1->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[1]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock2->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[2]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock3->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[3]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock4->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[4]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock5->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[5]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock6->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[6]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock7->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[7]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });
    takeStock7bis->setFunctCostAction([](GlobalState* globalState){
        return globalState->zoneStock[7]==ControlOwner::None && globalState->robotStatus != RobotStatus::full ? 1 : -1;
    });

    takeStock0->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock0bis->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock1->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock2->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock3->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock4->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock5->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock6->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock7->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });
    takeStock7bis->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
        globalState->commande = RobotStatus::full;
    });

    takeStock0->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock0bis->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock1->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock2->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock3->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock4->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock5->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock6->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock7->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });
    takeStock7bis->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return globalState->robotStatus == RobotStatus::full;
    });

    listeAction.push_back(takeStock0);
    listeAction.push_back(takeStock0bis);
    listeAction.push_back(takeStock1);
    listeAction.push_back(takeStock2);
    listeAction.push_back(takeStock3);
    listeAction.push_back(takeStock4);
    listeAction.push_back(takeStock5);
    listeAction.push_back(takeStock6);
    listeAction.push_back(takeStock7);
    listeAction.push_back(takeStock7bis);


    temperatureYellow->setStartPoint(TEMP_X_YELLOW_SART, TEMP_Y, 0, Direction::BACKWARD, Rotation::CLOCKWISE);
    temperatureBlue->setStartPoint(TEMP_X_BLUE_SART, TEMP_Y, 0, Direction::BACKWARD, Rotation::CLOCKWISE);

    temperatureYellow->setFunctGoodEnd([](GlobalState* globalState){
        globalState->termometre = true;
    });
    temperatureBlue->setFunctGoodEnd([](GlobalState* globalState){
        globalState->termometre = true;
    });

    temperatureYellow->setFunctCostAction([](GlobalState* globalState){
        return globalState->termometre == false && globalState->robotColor==ColorTeam::YELLOW ? 1 : -1;
    });
    temperatureBlue->setFunctCostAction([](GlobalState* globalState){
        return globalState->termometre == false && globalState->robotColor==ColorTeam::BLUE ? 1 : -1;
    });

    temperatureYellow->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return pushTemp(globalState, asserv, arduino);
    });
    temperatureBlue->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
        return pushTemp(globalState, asserv, arduino);;
    });

    listeAction.push_back(temperatureYellow);
    listeAction.push_back(temperatureBlue);

    map creatMap;
    for(int i = 0; i < creatMap.deposeCoord.size(); i++){
        for (size_t j = 0; j < creatMap.deposeCoord[i].size(); ++j) {
            const point_t& p = creatMap.deposeCoord[i][j];
            Action* deposeAction = new Action("deposeAction",globalState,asserv,arduino);
            deposeAction->setStartPoint (p.x, p.y, Direction::FORWARD);

            deposeAction->setFunctGoodEnd([i](GlobalState* globalState){
                globalState->zoneDepose[i] = ControlOwner::Friendly;
            });

            deposeAction->setFunctCostAction([i](GlobalState* globalState){
                return globalState->zoneDepose[i]==ControlOwner::None && globalState->robotStatus == RobotStatus::full ? 1 : -1;
            });

            deposeAction->setFunctRunAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino) {
                return globalState->robotStatus != RobotStatus::full;
            });

            deposeAction->setFunctStartAction([](GlobalState* globalState, Asserv* asserv, Arduino* arduino){
                globalState->commande = RobotStatus::empty;
            });

            listeAction.push_back(deposeAction);
        }
    }

}

int ActionContainer::actionContainerRun(void){
    if(currentAction == nullptr){
        //Choose first Action
        return choosNextAction();
    }
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
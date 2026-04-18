#include "GlobalState.h"


GlobalState::GlobalState(/* args */){
    startTimestamp = 0;
    for (auto& zone : zoneStock){
        zone = ControlOwner::None;
    }
    for (auto& zone : zoneStockEstrade){
        zone = ControlOwner::None;
    }
    for (auto& zone : zoneDepose){
        zone = ControlOwner::None;
    }
    termometre = false;
    robotStatus = RobotStatus::empty;
    commande = RobotStatus::empty;
    robotPosition = {0,0,0,0};
    robotColor = ColorTeam::NONE;
    enemyPosition = {0,0,0,0};
}

GlobalState::~GlobalState(){
}

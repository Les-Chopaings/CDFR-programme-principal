#pragma once
#include "utils.h"
#include "config.hpp"
#include "path_finding.h"

enum class ControlOwner{
    None,
    Friendly,
    Enemy
};

inline int xzoneDepose[10] = {1500, 1750,2200,2900,2300, 1500, 700,100,800,1250};
inline int yzoneDepose[10] = {800, 1450,800,800,100, 100, 100,800,800,1450};


//**************************************************************
//         |                                        |         //
//  YELLOW |                                        |   BLUE  //
//         |      SE0                   SE1         |         //
//------------------------------------------------------------//
//                D9                    D1                    //
//                                                            //
//                                                            //
//S6                                                        S1//
//                                                            //
//                                                            //
//                                                            //
//                                                            //
//D7         D8      S7      D0      S0      D2             D3//
//                                                            //
//                                                            //
//                                                            //
//S5                                                        S2//
//                                                            //
//                                                            //
//      D6        S4         D5       S3        D4            //
//**************************************************************

enum class RobotStatus{
    empty,
    full,
    reseting,
};

class GlobalState
{
public:

    //match
    long unsigned int startTimestamp = 0;

    //table
    ControlOwner zoneStock[8];
    ControlOwner zoneStockEstrade[2];
    ControlOwner zoneDepose[9];
    bool termometre;

    //robot
    RobotStatus commande = RobotStatus::empty;
    RobotStatus robotStatus = RobotStatus::empty;
    position_t robotPosition;
    ColorTeam robotColor;

    // robot ennemy
    position_t enemyPosition;

    PathFindingMap map;
    int collideDistance;

public:
    GlobalState(/* args */);
    ~GlobalState();
};
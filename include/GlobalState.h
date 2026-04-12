#pragma once
#include "utils.h"
#include "config.hpp"

enum class ControlOwner{
    None,
    Friendly,
    Enemy
};


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
    bool robotFull;
    bool robotIsSorted;
    position_t robotPosition;
    ColorTeam robotColor;

    // robot ennemy
    position_t enemyPosition;

public:
    GlobalState(/* args */);
    ~GlobalState();
};
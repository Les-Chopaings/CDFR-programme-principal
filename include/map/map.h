#pragma once


#define STOCK_X_0 1850
#define STOCK_Y_0 800
#define STOCK_X_1 2825
#define STOCK_Y_1 1200
#define STOCK_X_2 2825
#define STOCK_Y_2 400
#define STOCK_X_3 1900
#define STOCK_Y_3 175
#define STOCK_X_4 1100
#define STOCK_Y_4 175
#define STOCK_X_5 175
#define STOCK_Y_5 400
#define STOCK_X_6 175
#define STOCK_Y_6 1200
#define STOCK_X_7 1150
#define STOCK_Y_7 800

#define OFFSET_STOCK 200

#define STOCK_X_P0_0 STOCK_X_0
#define STOCK_Y_P0_0 STOCK_Y_0+OFFSET_STOCK
#define STOCK_T_P0_0 90

#define STOCK_X_P1_0 STOCK_X_0
#define STOCK_Y_P1_0 STOCK_Y_0-OFFSET_STOCK
#define STOCK_T_P1_0 -90

#define STOCK_X_P0_1 STOCK_X_1-OFFSET_STOCK
#define STOCK_Y_P0_1 STOCK_Y_1
#define STOCK_T_P0_1 0

#define STOCK_X_P0_2 STOCK_X_2-OFFSET_STOCK
#define STOCK_Y_P0_2 STOCK_Y_2
#define STOCK_T_P0_2 0

#define STOCK_X_P0_3 STOCK_X_3
#define STOCK_Y_P0_3 STOCK_Y_3+OFFSET_STOCK
#define STOCK_T_P0_3 90

#define STOCK_X_P0_4 STOCK_X_4
#define STOCK_Y_P0_4 STOCK_Y_4+OFFSET_STOCK
#define STOCK_T_P0_4 90

#define STOCK_X_P0_5 STOCK_X_5+OFFSET_STOCK
#define STOCK_Y_P0_5 STOCK_Y_5
#define STOCK_T_P0_5 180

#define STOCK_X_P0_6 STOCK_X_6+OFFSET_STOCK
#define STOCK_Y_P0_6 STOCK_Y_6
#define STOCK_T_P0_6 180

#define STOCK_X_P0_7 STOCK_X_7
#define STOCK_Y_P0_7 STOCK_Y_7+OFFSET_STOCK
#define STOCK_T_P0_7 90

#define STOCK_X_P1_7 STOCK_X_7
#define STOCK_Y_P1_7 STOCK_Y_7-OFFSET_STOCK
#define STOCK_T_P1_7 -90


#define STANDARD_NODES_ARRAY \
{0, {400, 1800}},   /*yellow start*/ \
{1, {2600, 1800}},  /*blue start*/ \
{2, {500, 1200}},   /*corner*/ \
{3, {500, 500}},    /*corner*/\
{4, {1100, 500}},   /*middle*/\
{5, {1150, 500}},   /*middle*/\
{6, {1850, 500}},   /*middle*/\
{7, {1900, 500}},   /*middle*/\
{8, {2500, 500}},   /*corner*/ \
{9, {2500, 1200}},  /*corner*/ \
{10, {1850, 1200}}, /*middle*/\
{11, {1150, 1200}}, /*middle*/\
{12, {400, 400}},   /*acces 0*/\
{13, {2600, 400}},  /*acces 0*/\
{14, {STOCK_X_P0_0, STOCK_Y_P0_0}},  /*stock 0*/\
{15, {STOCK_X_P0_1, STOCK_Y_P0_1}},  /*stock 1*/\
{16, {STOCK_X_P0_2, STOCK_Y_P0_2}},  /*stock 2*/\
{17, {STOCK_X_P0_3, STOCK_Y_P0_3}},  /*stock 3*/\
{18, {STOCK_X_P0_4, STOCK_Y_P0_4}},  /*stock 4*/\
{19, {STOCK_X_P0_5, STOCK_Y_P0_5}},  /*stock 5*/\
{20, {STOCK_X_P0_6, STOCK_Y_P0_6}},  /*stock 6*/\
{21, {STOCK_X_P0_7, STOCK_Y_P0_7}},  /*stock 7*/\
{22, {STOCK_X_P1_0, STOCK_Y_P1_0}},  /*stock 0*/\
{23, {STOCK_X_P1_7, STOCK_Y_P1_7}}   /*stock 7*/


#define STANDARD_EDGES_ARRAY \
{0, 2},\
{1, 9},\
{2, 3},\
{3, 4},\
{4, 5},\
{5, 6},\
{6, 7},\
{7, 8},\
{8, 9},\
{9, 10},\
{10, 11},\
{11, 2},\
{12, 3},\
{13, 8},\
{14, 10},\
{15, 9},\
{16, 13},\
{17, 7},\
{18, 4},\
{19, 12},\
{20, 2},\
{21, 11},\
{22, 6},\
{23, 5}

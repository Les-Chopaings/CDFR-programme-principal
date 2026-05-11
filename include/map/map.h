#pragma once
#include <cmath>
#include <cassert>
#include <cstdint>
#include <vector>
#include <array>
#include "config.hpp"

typedef struct {
    float x;
    float y;
} point_t;

typedef struct {
    int x;
    int y;
    int x_bis;
    int y_bis;
    int theta;
} point_angle_t;

typedef struct {
    uint8_t id;
    point_t point;
} node_t;

typedef struct {
    uint8_t start_node_id;
    uint8_t end_node_id;
    float distance;
    bool enabled;
} edge_t;

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

#define OFFSET_STOCK 250
#define OFFSET_DEPOSE 250
#define OFFSET_DEPOSE_BIS 300

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


#define DEPOSE_X_0 1500
#define DEPOSE_Y_0 800
#define DEPOSE_X_1 1750
#define DEPOSE_Y_1 1450
#define DEPOSE_X_2 2200
#define DEPOSE_Y_2 800
#define DEPOSE_X_3 2900
#define DEPOSE_Y_3 800
#define DEPOSE_X_4 2300
#define DEPOSE_Y_4 100
#define DEPOSE_X_5 1500
#define DEPOSE_Y_5 100
#define DEPOSE_X_6 700
#define DEPOSE_Y_6 100
#define DEPOSE_X_7 100
#define DEPOSE_Y_7 800
#define DEPOSE_X_8 800
#define DEPOSE_Y_8 800
#define DEPOSE_X_9 1250
#define DEPOSE_Y_9 1450

#define TEMP_Y 200
#define TEMP_X_YELLOW_SART 175
#define TEMP_X_YELLOW_END 700
#define TEMP_X_BLUE_SART 1700
#define TEMP_X_BLUE_END 2335-15

#define START_Y (2000-115)
#define START_X_YELLOW (600-203)
#define START_X_BLUE (2400+203-15)

#define STANDARD_NODES_ARRAY \
{0, {START_X_YELLOW, START_Y}},   /*yellow start*/ \
{1, {START_X_BLUE, START_Y}},  /*blue start*/ \
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
{12, {500, 400}},   /*acces 0*/\
{13, {2500, 400}},  /*acces 0*/\
{14, {STOCK_X_P0_0, STOCK_Y_P0_0}},  /*stock 0*/\
{15, {STOCK_X_P0_1, STOCK_Y_P0_1}},  /*stock 1*/\
{16, {STOCK_X_P0_2, STOCK_Y_P0_2}},  /*stock 2*/\
{17, {STOCK_X_P0_3, STOCK_Y_P0_3}},  /*stock 3*/\
{18, {STOCK_X_P0_4, STOCK_Y_P0_4}},  /*stock 4*/\
{19, {STOCK_X_P0_5, STOCK_Y_P0_5}},  /*stock 5*/\
{20, {STOCK_X_P0_6, STOCK_Y_P0_6}},  /*stock 6*/\
{21, {STOCK_X_P0_7, STOCK_Y_P0_7}},  /*stock 7*/\
{22, {STOCK_X_P1_0, STOCK_Y_P1_0}},  /*stock 0*/\
{23, {STOCK_X_P1_7, STOCK_Y_P1_7}},  /*stock 7*/\
{24, {TEMP_X_YELLOW_SART, TEMP_Y}},  /*temp */\
{25, {TEMP_X_YELLOW_END, TEMP_Y}},   /*temp */\
{26, {TEMP_X_BLUE_SART, TEMP_Y}},    /*temp */\
{27, {TEMP_X_BLUE_END, TEMP_Y}},     /*temp */\
{28, {TEMP_X_YELLOW_END,500}},       /*middle*/\
{29, {TEMP_X_BLUE_SART,500}},        /*middle*/\
{30, {START_X_YELLOW, 1550}},        /*yellow start*/ \
{31, {START_X_BLUE, 1550}},          /*blue start*/\
{32, {500,1000}},                    /*middle*/\
{33, {2500,1000}},                   /*middle*/\
{34, {2300, 500}},                   /*middle*/

#define LAST_POINT 34

#define STANDARD_EDGES_ARRAY \
{0, 30},\
{1, 31},\
{30, 2},\
{31, 9},\
{2, 32},\
{32, 3},\
{3, 28},\
{28, 4},\
{4, 5},\
{5, 29},\
{29, 6},\
{6, 7},\
{7, 34},\
{34, 8},\
{8, 33},\
{33, 9},\
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
{23, 5},\
{24,25},\
{26,27},\
{24,3},\
{25,28},\
{26,29},\
{8,27}


class map
{
private:
    /* data */
    std::vector<node_t> *m_Nodes;
    std::vector<edge_t> *m_Edges;
public:
    map(/* args */);
    ~map();

    std::vector<node_t>* get_node_arry();
    std::vector<edge_t>* get_edge_arry();
    std::array<std::vector<point_angle_t>, 9> deposeCoord;

private:
    point_angle_t point_at_distance(float x1, float y1, float x2, float y2, float dist, float dist_bis);
    point_angle_t add_a_point(uint8_t firstPointId, float x1, float y1, uint8_t pointId1, float dist = 0, float distBis=0);
};


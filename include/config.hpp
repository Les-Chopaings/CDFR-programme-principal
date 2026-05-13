#pragma once

#define RAD_TO_DEG 57.29577951
#define DEG_TO_RAD 0.01745329252

#define ROBOT_X_OFFSET 155
#define ROBOT_Y_OFFSET 60

#if defined(__i386__) || defined(__x86_64__)
    #define EMULATE
#endif

#define DISTANCE_COLLIDE 300
#define DISTANCE_DECOLLIDE 350
#define TIME_BEFORE_RESTART 1500
#define TIME_BEFORE_ABANDON 1000000

#define DEFAULT_ANGLE 130
#define SLIDER_OFFSET_1_POS(offset) (offset+10)
#define SLIDER_OFFSET_2_POS(offset) (offset+3)
#define SLIDER_OFFSET_3_POS(offset) (offset-3)
#define SLIDER_OFFSET_4_POS(offset) (offset-15)

#define SLIDER_1_POS (DEFAULT_ANGLE+10)
#define SLIDER_2_POS (DEFAULT_ANGLE+3)
#define SLIDER_3_POS (DEFAULT_ANGLE-3)
#define SLIDER_4_POS (DEFAULT_ANGLE-15)

typedef struct
{
    int   x;
    int   y;
    int   theta;
    int   time;
}position_t;

typedef struct
{
    bool   valid;
    double   angle;
    double   dist;
    int   x;
    int   y;
    bool   onTable;
}lidarAnalize_t;

enum class ColorTeam{
    NONE,
    BLUE,
    YELLOW
};

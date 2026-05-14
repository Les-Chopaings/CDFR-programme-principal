#include <signal.h>

#include "lidarAnalize.hpp"
#include "lidar.hpp"
#include "Arduino.hpp"
#include "Colorsensor.hpp"
#include "asserv/asserv.h"
#include "utils.h"
#include "GlobalState.h"
#include "Traceur.hpp"
#include "ActionContainer.hpp"
#include "Fsm.hpp"
#include "espSerial.hpp"

#include "path_finding.h"
#include <chrono>

#define SIZEDATALIDAR 10000


bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}
bool ctrl_z_pressed = false;
void ctrlz(int signal) {
    ctrl_z_pressed = true;
}

enum class MainState{
    INIT,
    INITIALIZE,
    SETHOME,
    WAITSTART,
    START,
    RUN,
    RETURNHOME,
    FIN,
    STOP
};

void stopAndRest(Asserv* asserv, Arduino* arduino){
    asserv->stop();
    asserv->set_coordinates(0,0,0);
    asserv->set_motor_state(false);
    asserv->set_brake_state(false);
    arduino->controlePompe(pompe::pompe1,0);
    arduino->controlePompe(pompe::pompe2,0);
    arduino->controlePompe(pompe::pompe3,0);
    arduino->controlePompe(pompe::pompe4,0);
    arduino->stepperEnable(false);
    arduino->stepperMove(0);
    arduino->servoMove(servo::bascule,0);
    arduino->servoMove(servo::temp,0);
    arduino->servoAllEnable(false);
};

void test_path_finder() {
    auto map = PathFindingMap();

    for (auto edge: map.found_edges_intersecting_rectangle(1000,1000,500,500)) {
        std::cout << "Removing edge: " << static_cast<int>(edge.start_node_id) << " <-> " << static_cast<int>(edge.end_node_id) << std::endl;
        map.toggle_edge_between_two_nodes(edge.start_node_id, edge.end_node_id, false);
    }

    map.update_base_map();


    const auto start = std::chrono::high_resolution_clock::now();
    path_t path = map.find_path_between_points(
        {400, 1800, 255, M_PI_2},
        {1150, 750, 255, -M_PI},
        true
    );
    const auto end = std::chrono::high_resolution_clock::now();

    std::cout << "cmpt time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    for (auto [point, node_id, theta]: path.v) {
        std::cout << "Node: " << static_cast<int>(node_id) << " (" << point.x << ", " << point.y << " @ "
                << theta * 180 / M_PI << ")"
                << std::endl;
    }

    Traceur mat(3000, 2000, 50);

    // Test ligne
    for (edge_out_t &edge: map.get_edge_map()) {
        mat.drawLine(edge.start_point.x, edge.start_point.y, edge.end_point.x, edge.end_point.y, edge.enabled+1);
    }
    LOG_DEBUG("=>",path.length);

    for (size_t i = 0; i < path.v.size(); ++i) {
        auto [point, node_id, theta] = path.v[i];

        uint8_t val = 3;

        if (i == 0)
            val = 4;           // premier point
        else if (i == path.v.size() - 1)
            val = 5;           // dernier point

        mat.drawPoint(point.x,
                    point.y,
                    val);
    }

    mat.print();
}


int main(int argc, char *argv[]) {
    LOG_INIT();

#ifdef EMULATE
    Arduino arduino(-1);
    Asserv asserv(-1);
    Colorsensor colorsensor(-1, -1);
    bool emulate = true;
#else
    Arduino arduino(0x64);
    Colorsensor colorsensor(TCS34725_ADDRESS, TCAADDR);
    Asserv asserv(42);
    ESPSerial esp("/dev/ttyUSB0");
    if(!lidarSetup("/dev/ttyUSB1",460800)){
        LOG_ERROR("cannot find the lidar");
        return -1;
    }
    bool emulate = false;
#endif
    lidarAnalize_t lidarData[SIZEDATALIDAR];
    GlobalState globalState;
    globalState.map = new PathFindingMap();
    globalState.map->update_base_map();
    ActionContainer actionContainer(&globalState, &asserv, &arduino);
    Fsm fsm;

    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);
    signal(SIGTSTP, ctrlz);
    usleep(300000);
    MainState currentState = MainState::INIT;
    MainState nextState = currentState;
    bool initStat = true;
    bool ledToggle = false;
    unsigned long timeLedToggle = 0;

#ifdef EMULATE
    // PRINT MAP (only for debug purpose)
    std::string content = globalState.map->get_Map(5);
    std::filesystem::path exeDir = getExecutableDir(argv[0]);
    std::filesystem::path filePath = exeDir / "map_output.txt";
    std::ofstream file(filePath);
    if (!file) {
        std::cerr << "Erreur ouverture fichier\n";
        return 1;
    }
    file << content;
#endif

    // TEST
    // asserv.go_to_point(1000, 0, 0);

    // Demo path finding
    // test_path_finder();

    //arduino.controlePompe(pompe::pompe1,1);
    //arduino.controlePompe(pompe::pompe2,1);
    //arduino.controlePompe(pompe::pompe3,1);
    //arduino.controlePompe(pompe::pompe4,1);

    while (1) {
        //bool rotation[4];
        //fsm.readSensor(rotation,ColorTeam::YELLOW,&colorsensor);

        LOG_SCOPE("Main");

        if(timeLedToggle < millis()){
            if(LOG_GET_ERROR()){
                timeLedToggle = millis()+250;
            }
            else{
                timeLedToggle = millis()+1000;
            }
            ledToggle = !ledToggle;
            arduino.ledEnable(ledToggle);
        }

        //Aquistion
        int16_t x, y, theta;
        bool dataValid = false;
        asserv.get_coordinates(x, y, theta);
        globalState.robotPosition.x = x;
        globalState.robotPosition.y = y;
        globalState.robotPosition.theta = theta;
        LOG_SET_COORD(x, y, theta);
        if(globalState.startTimestamp !=0){
            LOG_SET_TIME((millis() - globalState.startTimestamp)/1000);
        }
#ifndef EMULATE
        globalState.distanceKappla = esp.getLastValue();
#endif
        //LOG_DEBUG("DISTANCE : ",globalState.distanceKappla);

        int count = SIZEDATALIDAR;
        if(getlidarData(lidarData,count)){
            rotateLidarData(lidarData, count, -45);
            convertAngularToAxial(lidarData,count,globalState.robotPosition);
            dataValid = true;
        }
        #ifdef EMULATE
        {
            position_t enemyPosition = {5000,5000,theta,0};
            globalState.enemyPosition = enemyPosition;
            placeVirtualEnemy(lidarData,count,globalState.robotPosition,enemyPosition);
            dataValid = true;
        }
        #endif

        if(dataValid){
            if(ctrl_z_pressed){
                ctrl_z_pressed = false;
                pixelArtPrint(lidarData,count,3000,2000,50,globalState.robotPosition);
            }
            Direction asservDir = asserv.get_direction_side();
            int direction = asservDir==Direction::FORWARD ? 1 : (asservDir==Direction::BACKWARD ? -1 : 0);
            direction = getDirection(direction);
            globalState.collideDistance = collide(lidarData,count,direction);
            if(direction == 1){
                globalState.collideDistance = globalState.collideDistance - 220;
            }
            else{
                globalState.collideDistance = globalState.collideDistance - 120;
            }
        }

        switch (currentState) {
            //****************************************************************
            // wait to start the initialisation process
            case MainState::INIT:{
                if(initStat){
                    LOG_STATE("INIT");
                    //TODO
                }
                if(arduino.readButton(button::magnet) || 1 || emulate){
                    nextState = MainState::INITIALIZE;
                }
                break;
            }
            //****************************************************************
            // initialisation first step : enable all actuator
            case MainState::INITIALIZE:{
                if(initStat){
                    LOG_STATE("INITIALIZE");
                    if(arduino.readButton(button::color)){
                        globalState.robotColor = ColorTeam::BLUE;
                        asserv.set_coordinates(START_X_BLUE,START_Y,90);
                    }
                    else{
                        globalState.robotColor = ColorTeam::YELLOW;
                        asserv.set_coordinates(START_X_YELLOW,START_Y,90);
                    }
                    asserv.set_motor_state(true);
                    asserv.set_brake_state(false);
                    arduino.stepperEnable(true);
                    arduino.servoAllEnable(true);
                }
                nextState = MainState::SETHOME;
                break;
            }
            //****************************************************************
            // initialisation second step : set home
            case MainState::SETHOME:{
                if(initStat){
                    LOG_STATE("SETHOME");
                    arduino.stepperMove(700);
                    arduino.servoMove(servo::slider1,0);
                    arduino.servoMove(servo::slider2,0);
                    arduino.servoMove(servo::slider3,0);
                    arduino.servoMove(servo::slider4,0);
                    arduino.servoMove(servo::slider4,SLIDER_4_POS);
                    arduino.servoMove(servo::slider3,SLIDER_3_POS);
                    arduino.servoMove(servo::slider2,SLIDER_2_POS);
                    arduino.servoMove(servo::slider1,SLIDER_1_POS);
                    usleep(500000);
                    arduino.servoMove(servo::bascule,180);
                }
                nextState = MainState::WAITSTART;
                break;
            }
            //****************************************************************
            // wait start from magnet
            case MainState::WAITSTART:{
                if(initStat){
                    LOG_STATE("WAITSTART");
                }
                if(!arduino.readButton(button::magnet) || emulate){
                    nextState = MainState::START;
                }
                break;
            }
            //****************************************************************
            // enable all system and start timer
            case MainState::START:{
                if(initStat){
                    LOG_STATE("START");
                }
                globalState.startTimestamp = millis();
                nextState = MainState::RUN;
                break;
            }
            //****************************************************************
            // action to play during 100s
            case MainState::RUN:{
                if(initStat) LOG_STATE("RUN");
                int finish = 0;
                fsm.takeNutsRun(&globalState, &asserv, &arduino, &colorsensor);
                finish = actionContainer.actionContainerRun();
                if((globalState.startTimestamp + 100000) < millis()){
                    LOG_GREEN_INFO("END BY TIMER");
                    nextState = MainState::FIN;
                }
                if(finish<0){
                    LOG_GREEN_INFO("END BY ACTION");
                    nextState = MainState::FIN;
                }
                break;
            }
            //****************************************************************
            // disable all actuator
            case MainState::FIN:
                if(initStat){
                    stopAndRest(&asserv,&arduino);
                    LOG_STATE("FIN");
                }
                nextState = MainState::STOP;
                break;
            //****************************************************************
            // END!!
            case MainState::STOP:
                if(initStat){
                    LOG_STATE("STOP");
                }
                break;
            //****************************************************************
            default:
                LOG_STATE("default");
                nextState = MainState::STOP;
                break;
        }

        initStat = false;
        if(currentState != nextState){
            initStat = true;
        }
        currentState = nextState;

        if (ctrl_c_pressed){
            break;
        }

        usleep(10000);
    }

    LOG_DEBUG("PROCESS KILL");
    stopAndRest(&asserv,&arduino);
    lidarStop();

    return 0;
}
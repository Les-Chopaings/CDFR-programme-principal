#include "Fsm.hpp"

Fsm::Fsm(/* args */)
{
}

Fsm::~Fsm()
{
}


void Fsm::readSensor(bool* rotation, ColorTeam colorteam, Colorsensor* colorsensor){
    RGBColor blue = {38,85,132};
    RGBColor yellow = {110,103,42};
    float similarBlue;
    float similarYellow;
    ColorTeam noisetteColor;
    colorsensor->readAllSensor();
    std::ostringstream debugColor;
    std::ostringstream debugColorValue;
    for (int i = 0; i < 4; i++){
        similarBlue = colorsensor->compareColors(i,blue);
        similarYellow = colorsensor->compareColors(i,yellow);
        if(similarBlue>similarYellow){
            noisetteColor = ColorTeam::BLUE;
            debugColor << "\033[34m";
            debugColor << "BLUE";
            debugColor << "\033[0m";
        }
        else{
            noisetteColor = ColorTeam::YELLOW;
            debugColor << "\033[33m";
            debugColor << "YELLOW";
            debugColor << "\033[0m";
        }
        debugColorValue << "B:" + std::to_string(similarBlue) + " Y:" + std::to_string(similarYellow);
        if(i != 4){
            debugColor << " ";
            debugColorValue << " | ";
        }
        rotation[i] = colorteam != noisetteColor ? true : false;
    }
    LOG_DEBUG(debugColor.str());
    LOG_DEBUG(debugColorValue.str());
}

bool Fsm::takeNutsRun(GlobalState* globalState, Asserv* asserv, Arduino* arduino, Colorsensor* colorsensor){
    FsmTakeNuts nextState = currentState;
    bool bret = false;

    switch (currentState){
        case FsmTakeNuts::INIT :{
            if(globalState->commande == RobotStatus::empty){
                nextState = FsmTakeNuts::RESET_DOWN;
            }
            break;
        }
        case FsmTakeNuts::RESET_DOWN :{
            if(initStat){
                startTime = millis()+1000;
                arduino->stepperMove(150);
                arduino->servoMove(servo::bascule,0);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::TAKE_WAIT;
                globalState->robotStatus = RobotStatus::empty;
            }
            break;
        }
        case FsmTakeNuts::TAKE_WAIT :{
            if(globalState->commande == RobotStatus::full){
                nextState = FsmTakeNuts::TAKE_DOWN;
                int16_t x, y, theta;
                asserv->get_coordinates(x, y, theta);
                LOG_DEBUG("COOR: ",x," ",y," ",theta);
            }
            break;
        }
        case FsmTakeNuts::TAKE_DOWN :{
            if(initStat){
                startTime = millis()+200;
                arduino->controlePompe(pompe::pompe1,1);
                arduino->controlePompe(pompe::pompe2,1);
                arduino->controlePompe(pompe::pompe3,1);
                arduino->controlePompe(pompe::pompe4,1);
                arduino->stepperMove(35);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::TAKE_LEFT;
            }
            break;
        }
        case FsmTakeNuts::TAKE_LEFT :{
            if(initStat){
                startTime = millis()+2000;
                arduino->servoMove(servo::rotation1,15);
                arduino->servoMove(servo::rotation2,15);
                arduino->servoMove(servo::rotation3,15);
                arduino->servoMove(servo::rotation4,15);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::TAKE_RIGHT;
            }
            break;
        }
        case FsmTakeNuts::TAKE_RIGHT :{
            if(initStat){
                startTime = millis()+2000;
                arduino->servoMove(servo::rotation1,0);
                arduino->servoMove(servo::rotation2,0);
                arduino->servoMove(servo::rotation3,0);
                arduino->servoMove(servo::rotation4,0);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::TAKE_UP;
            }
            break;
        }
        case FsmTakeNuts::TAKE_UP :{
            if(initStat){
                startTime = millis()+2000;
                arduino->stepperMove(1150);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::TAKE_PIVOT_90;
            }
            break;
        }
        case FsmTakeNuts::TAKE_PIVOT_90 :{
            if(initStat){
                startTime = millis()+2000;
                arduino->servoMove(servo::bascule,180);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::READ_SENSOR;
            }
            break;
        }
        case FsmTakeNuts::READ_SENSOR:{
            readSensor(rotation, globalState->robotColor, colorsensor);
            nextState = FsmTakeNuts::SORT;
            break;
        }
        case FsmTakeNuts::SORT :{
            if(initStat){
                globalState->robotStatus = RobotStatus::full;
            }
            if(TriNoisette(rotation, arduino)){
                nextState = FsmTakeNuts::WAIT_PUT;
            }
            break;
        }
        case FsmTakeNuts::WAIT_PUT:{
            if(globalState->commande==RobotStatus::reseting){
                nextState = FsmTakeNuts::PUT_PIVOT_45;
            }
            break;
        }
        case FsmTakeNuts::PUT_PIVOT_45 :{
            if(initStat){
                startTime = millis()+200;
                arduino->servoMove(servo::bascule,110);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::PUT_DOWN;
            }
            break;
        }
        case FsmTakeNuts::PUT_DOWN :{
            if(initStat){
                startTime = millis()+500;
                arduino->stepperMove(700);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::PUT_STOP_POMPE;
            }
            break;
        }
        case FsmTakeNuts::PUT_STOP_POMPE :{
            if(initStat){
                startTime = millis()+1000;
                arduino->controlePompe(pompe::pompe1,0);
                arduino->controlePompe(pompe::pompe2,0);
                arduino->controlePompe(pompe::pompe3,0);
                arduino->controlePompe(pompe::pompe4,0);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::PUT_PIVOT_90;
            }
            break;
        }
        case FsmTakeNuts::PUT_PIVOT_90 :{
            if(initStat){
                startTime = millis()+500;
                arduino->servoMove(servo::bascule,180);
            }
            if(startTime < millis()){
                nextState = FsmTakeNuts::RESET_SORT;
                globalState->robotStatus = RobotStatus::reseting;
            }
            break;
        }
        case FsmTakeNuts::RESET_SORT :{
            bool rot[4] = {0,0,0,0};
            if(TriNoisette(rot, arduino)){
                nextState = FsmTakeNuts::RESET_WAIT_DOWN;
            }
            break;
        }
        default:
            nextState = FsmTakeNuts::INIT;
            globalState->robotStatus = RobotStatus::reseting;
            break;
    }
    initStat = false;
    if(nextState != currentState){
        LOG_STATE(FsmTakeNuts_to_string(nextState));
        initStat = true;
    }
    currentState = nextState;
    return bret;
}

#define QUIET
int Fsm::TriNoisette(bool* rotate, Arduino* arduino){
    switch (triCurrentState) {

        /*Etat idle, vérification des kapla à retourner*/
        default:{
#ifndef QUIET
            LOG_DEBUG("idle");
            LOG_DEBUG("verification de la liste initiale= ", rotate[0], rotate[1], rotate[2], rotate[3]);
#endif
            /* si besoin de retourner des kaplas:*/
            if (rotate[0] == 1 || rotate[1] == 1 || rotate[2] == 1 || rotate[3] == 1){
                regular = 1;}
            /* si appel reset*/
            else if(rotate[0] == 0 && rotate[1] == 0 && rotate[2] == 0 && rotate[3] == 0){
                /*mais que l'état d'avant était déjà reset: on saute*/
                if(regular == 0){return 1;}
                else{regular = 0;}
            }
            finished = 0; toCopy = 1;
            servoToMove[0] = 0; servoToMove[1] = 180; servoToMove[2] = 180; servoToMove[3] = 180;
            triCurrentState = State::init;
            break;}

        /*case toProcess : gère l'appel des cases 'rotateX' en fonction des bras à retourner*/
        case State::toProcess:{
            /* Des kaplas doivent être retournés, on copie la liste des kaplas */
#ifndef QUIET
            LOG_DEBUG("toProcess, status regular = ", regular, toCopy);
#endif
            if (toCopy){
                for (int i = 0; i <= 3; i++){
                    /* récupère les bras à retourner*/
                    if (regular){servoToRot[i] = rotate[i];}
                    /* sinon, dans le mode reverse:*/
                    else{servoToRot[i] = servoCurrentRot[i];}
                    }
                toCopy = 0;}

            // vérification des kaplas à retourner
#ifndef QUIET
            LOG_DEBUG("verification de la liste en attente = ", servoToRot[0], servoToRot[1], servoToRot[2], servoToRot[3]);
            LOG_DEBUG("vérification de la position des bras = ", servoCurrentRot[0], servoCurrentRot[1], servoCurrentRot[2], servoCurrentRot[3]);
#endif
            /* déplace les bras pour rotation1 || reset bras 1*/
            if (servoToRot[0] == 1){
                followupState = State::rotate1;
                servoToRot[0] = 0;}
            /* déplace les bras pour rotation2*/
            else if (servoToRot[1] == 1){
                servoToMove[0] = 0; servoToMove[1] = 80; servoToMove[2] = 180; servoToMove[3] = 180;
                followupState = State::rotate2;
                servoToRot[1] = 0;}
            /* déplace les bras pour rotation3*/
            else if (servoToRot[2] == 1){
                servoToMove[0] = 0; servoToMove[1] = 0; servoToMove[2] = 80; servoToMove[3] = 180;
                followupState = State::rotate3;
                servoToRot[2] = 0;}
            /* déplace les bras pour rotation4*/
            else if (servoToRot[3] == 1){
                servoToMove[0] = 0; servoToMove[1] = 0; servoToMove[2] = 0; servoToMove[3] = 180;
                followupState = State::rotate4;
                servoToRot[3] = 0;}
            /* si plus aucun  kapla a retourner*/
            else {
#ifndef QUIET
                LOG_DEBUG("retourne à l'état init");
#endif
                servoToMove[0] = 0; servoToMove[1] = 0; servoToMove[2] = 0; servoToMove[3] = 180;
                finished = 1;
            }

            triCurrentState = State::movement;
            break;}

        case State::rotate1:{
#ifndef QUIET
            LOG_DEBUG("retourne le kapla 1, regular status: ", regular);
#endif
            arduino->servoMove(servo::rotation1, 180*regular);
            T_start =  millis();
            servoCurrentRot[0] = regular;
            triCurrentState = State::rotwait;
            break;}

        case State::rotate2:{
#ifndef QUIET
            LOG_DEBUG("retourne le kapla 2, regular status: ", regular);
#endif
            arduino->servoMove(servo::rotation2, 180*regular);
            T_start =  millis();
            servoCurrentRot[1] = regular;
            triCurrentState = State::rotwait;
            break;}

        case State::rotate3:{
#ifndef QUIET
            LOG_DEBUG("retourne le kapla 3, regular status: ", regular);
#endif
            arduino->servoMove(servo::rotation3, 180*regular);
            T_start =  millis();
            servoCurrentRot[2] = regular;
            triCurrentState = State::rotwait;
            break;}

        case State::rotate4:{
#ifndef QUIET
            LOG_DEBUG("retourne le kapla 4, regular status: ", regular);
#endif
            arduino->servoMove(servo::rotation4, 180*regular);
            T_start =  millis();
            servoCurrentRot[3] = regular;
            triCurrentState = State::rotwait;
            break;}

        /*case mouvement : déplacement des bras 1 puis 2, 3, 4 (déplacement uniquement si nécessaire)*/
        case State::movement:{
            /* bouge le servo 1 si nécessaire*/
            if(servoToMove[0] != servoCurrentPos[0]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 1 à ", servoToMove[0]);
#endif
                arduino->servoMove(servo::slider1, servoToMove[0]);
                servoCurrentPos[0] = servoToMove[0];
                T_start =  millis();
                triCurrentState = State::wait;
            }
            /* bouge le servo 2 si nécessaire*/
            else if(servoToMove[1] != servoCurrentPos[1]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 2 à ", servoToMove[1]);
#endif
                arduino->servoMove(servo::slider2, servoToMove[1]);
                servoCurrentPos[1] = servoToMove[1];
                T_start =  millis();
                triCurrentState = State::wait;
            }
            /* bouge le servo 3 si nécessaire*/
            else if(servoToMove[2] != servoCurrentPos[2]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 3 à ", servoToMove[2]);
#endif
                arduino->servoMove(servo::slider3, servoToMove[2]);
                servoCurrentPos[2] = servoToMove[2];
                T_start =  millis();
                triCurrentState = State::wait;
            }
            /* bouge le servo 4 si nécessaire*/
            else if(servoToMove[3] != servoCurrentPos[3]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 4 à ", servoToMove[3]);
#endif
                arduino->servoMove(servo::slider4, servoToMove[3]);
                servoCurrentPos[3] = servoToMove[3];
                T_start =  millis();
                triCurrentState = State::wait;
            }
            /* si finished, on recale les bras */
            else if (finished){
                servoToMove[0] = 90; servoToMove[1] = 90; servoToMove[2] = 90; servoToMove[3] = 90;
                triCurrentState = State::init;
            }
            else{
                triCurrentState = followupState;
            }
            break;}
        /*case wait : attente liée au déplacement d'un bras */
        case State::wait:{
            int fs = millis() - T_start;
            if(fs > TIMESLIDER){
#ifndef QUIET
                LOG_DEBUG("wait ", TIMESLIDER, " ms");
#endif
                triCurrentState = State::movement;
            }
        break;}
        /*case waitinit : attente liée au déplacement d'un bras, mais pour init*/
        case State::waitinit:{
            int fs = millis() - T_start;
            if(fs > TIMESLIDER){
#ifndef QUIET
                LOG_DEBUG("waitinit ", TIMESLIDER, " ms");
#endif
                triCurrentState = State::init;
            }
        break;}
        /*case rotwait : attente liée à la rotation d'un bras*/
        case State::rotwait:{
            int fs = millis() - T_start;
            if(fs > TIMEROTATE){
#ifndef QUIET
                LOG_DEBUG("wait ", TIMEROTATE, " ms");
#endif
                triCurrentState = State::toProcess;
            }
        break;}
        /*case init : uniquement au début et à la fin (commence par le bras n°4, puis 3, 2, 1)*/
        case State::init:{
            /*On commence par le servo 4, puis 3 ,2 et 1*/
            if(servoToMove[3] != servoCurrentPos[3]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 4 à ", servoToMove[3]);
#endif
                arduino->servoMove(servo::slider4, servoToMove[3]);
                servoCurrentPos[3] = servoToMove[3];
                T_start =  millis();
                triCurrentState = State::waitinit;
            }
            else if(servoToMove[2] != servoCurrentPos[2]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 3 à ", servoToMove[2]);
#endif
                arduino->servoMove(servo::slider3, servoToMove[2]);
                servoCurrentPos[2] = servoToMove[2];
                T_start =  millis();
                triCurrentState = State::waitinit;
            }
            else if(servoToMove[1] != servoCurrentPos[1]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 2 à ", servoToMove[1]);
#endif
                arduino->servoMove(servo::slider2, servoToMove[1]);
                servoCurrentPos[1] = servoToMove[1];
                T_start =  millis();
                triCurrentState = State::waitinit;
            }
            else if(servoToMove[0] != servoCurrentPos[0]){
#ifndef QUIET
                LOG_DEBUG("bouge servo 1 à ", servoToMove[0]);
#endif
                arduino->servoMove(servo::slider1, servoToMove[0]);
                servoCurrentPos[0] = servoToMove[0];
                T_start =  millis();
                triCurrentState = State::waitinit;
            }
            else if(finished){
                LOG_GREEN_INFO("---Tri fini---");
                triCurrentState = State::idle;
                /*finished = 1*/
                return(finished);
            }
            else {triCurrentState = State::toProcess;}
        break;}
    }

    return (0);   /* sortie de la boucle (= 0)*/
}


int pushTemp(GlobalState* globalState, Asserv* asserv, Arduino* arduino){
    static FsmTemp currentState = FsmTemp::INIT;
    static int initStat = false;
    static unsigned long startTime = 0;
    FsmTemp nextState = currentState;
    bool bret = false;
    static int deplacementreturn = 0;

    switch (currentState){
        case FsmTemp::INIT :
            nextState = FsmTemp::DEPLOY;
            globalState->termometre = true;
            break;

        case FsmTemp::DEPLOY :
            if(initStat){
                startTime = millis()+500;
                arduino->servoMove(servo::temp,180);
            }
            if(startTime < millis()){
                nextState = FsmTemp::FORWARD;
            }
            break;

        case FsmTemp::FORWARD :
            if(globalState->robotColor == ColorTeam::YELLOW){
                asserv->go_to_point(TEMP_X_YELLOW_END, TEMP_Y);
            }
            else{
                asserv->go_to_point(TEMP_X_BLUE_END, TEMP_Y);
            }
            nextState = FsmTemp::WAIT;
            break;

        case FsmTemp::RETRACT :
            if(initStat){
                startTime = millis()+500;
                arduino->servoMove(servo::temp,0);
            }
            if(startTime < millis()){
                bret = deplacementreturn;
                nextState = FsmTemp::INIT;
            }
            break;

        case FsmTemp::WAIT :
            if(globalState->collideDistance<DISTANCE_COLLIDE){
                nextState = FsmTemp::COLIDE;
                startTime = millis() + TIME_BEFORE_RESTART;
                asserv->pause();
            }
            else if(asserv->get_moving_is_done() && asserv->get_command_buffer_size() == 0){
                nextState = FsmTemp::RETRACT;
                deplacementreturn = 1;
            }
            break;

        case FsmTemp::WAIT_COLIDE :{
            if(startTime<millis()){
                nextState = FsmTemp::COLIDE;
                startTime = millis() + TIME_BEFORE_ABANDON;
            }
            break;
        }

        case FsmTemp::COLIDE :
            if(globalState->collideDistance>DISTANCE_DECOLLIDE){
                nextState = FsmTemp::WAIT;
                asserv->resume();
            }
            else if(startTime<millis()){
                nextState = FsmTemp::RETRACT;
                asserv->stop();
                deplacementreturn = -1;
            }
            break;

        default:
            nextState = FsmTemp::INIT;
            break;
    }


    initStat = false;
    if(nextState != currentState){
        LOG_STATE(FsmTemp_to_string(nextState));
        initStat = true;
    }
    currentState = nextState;
    return bret;
}



int takeForaward(GlobalState* globalState, Asserv* asserv, Arduino* arduino, int x, int y){
    static FsmTakeForaward currentState = FsmTakeForaward::INIT;
    static int initStat = false;
    static unsigned long startTime = 0;
    FsmTakeForaward nextState = currentState;
    int deplacementreturn = 0;

    switch (currentState){
        case FsmTakeForaward::INIT :
            globalState->commande = RobotStatus::empty;
            nextState = FsmTakeForaward::WAIT_FORWARD;
            break;

        case FsmTakeForaward::WAIT_FORWARD :
            // wait end reseting
            if(globalState->robotStatus == RobotStatus::empty){
                nextState = FsmTakeForaward::FORWARD;
            }
            break;

        case FsmTakeForaward::FORWARD :
            asserv->go_to_point(x, y, Rotation::SHORTEST ,Direction::FORWARD);
            nextState = FsmTakeForaward::WAIT;
            break;

        case FsmTakeForaward::WAIT :
            if(globalState->collideDistance<DISTANCE_COLLIDE){
                nextState = FsmTakeForaward::COLIDE;
                startTime = millis() + TIME_BEFORE_RESTART;
                asserv->pause();
            }
            else if(asserv->get_moving_is_done() && asserv->get_command_buffer_size() == 0){
                nextState = FsmTakeForaward::WAIT_TAKE;
            }
            break;

        case FsmTakeForaward::WAIT_COLIDE :{
                if(startTime<millis()){
                    nextState = FsmTakeForaward::COLIDE;
                    startTime = millis() + TIME_BEFORE_ABANDON;
                }
                break;
            }

        case FsmTakeForaward::COLIDE :
            if(globalState->collideDistance>DISTANCE_DECOLLIDE){
                nextState = FsmTakeForaward::WAIT;
                asserv->resume();
            }
            else if(startTime<millis()){
                nextState = FsmTakeForaward::INIT;
                asserv->stop();
                deplacementreturn = -1;
            }
            break;

        case FsmTakeForaward::WAIT_TAKE :
            if(initStat){
                globalState->commande = RobotStatus::full;
            }
            if(globalState->robotStatus == RobotStatus::full){
                nextState = FsmTakeForaward::INIT;
                deplacementreturn = 1;
            }
            break;

        default:
            nextState = FsmTakeForaward::INIT;
            break;
    }


    initStat = false;
    if(nextState != currentState){
        LOG_STATE(FsmTakeForaward_to_string(nextState));
        initStat = true;
    }
    currentState = nextState;
    return deplacementreturn;
}
#include "Minifsm.hpp"
using namespace std;

Minifsm::Minifsm(/* args */)
{
}

Minifsm::~Minifsm()
{
}


int Minifsm::TriNoisette(bool* rotate, Arduino* arduino)
{
switch (currentState) {

            /*Etat idle, vérification des kapla à retourner*/
            default:{
                LOG_DEBUG("idle");
                LOG_DEBUG("verification de la liste initiale= ", rotate[0], rotate[1], rotate[2], rotate[3]);
                /* si besoin de retourner des kaplas:*/
                if (rotate[0] == 1 || rotate[1] == 1 || rotate[2] == 1 || rotate[3] == 1){
                    regular = 1;}
                /* si appel reset*/
                else if(rotate[0] == 0 && rotate[1] == 0 && rotate[2] == 0 && rotate[3] == 0){
                    /*mais que l'état d'avant était déjà reset: on saute*/
                    if(regular == 0){break;}
                    else{regular = 0;}
                }
                finished = 0; toCopy = 1;
                servoToMove[0] = 0; servoToMove[1] = 180; servoToMove[2] = 180; servoToMove[3] = 180;
                currentState = State::init;
                break;}

            /*case toProcess : gère l'appel des cases 'rotateX' en fonction des bras à retourner*/
            case State::toProcess:{
                /* Des kaplas doivent être retournés, on copie la liste des kaplas */
                LOG_DEBUG("toProcess, status regular = ", regular, toCopy);
                if (toCopy){
                    for (int i = 0; i <= 3; i++){
                        /* récupère les bras à retourner*/
                        if (regular){servoToRot[i] = rotate[i];}
                        /* sinon, dans le mode reverse:*/
                        else{servoToRot[i] = servoCurrentRot[i];}
                        }
                    toCopy = 0;}

                // vérification des kaplas à retourner
                LOG_DEBUG("verification de la liste en attente = ", servoToRot[0], servoToRot[1], servoToRot[2], servoToRot[3]);
                LOG_DEBUG("vérification de la position des bras = ", servoCurrentRot[0], servoCurrentRot[1], servoCurrentRot[2], servoCurrentRot[3]);
                /* déplace les bras pour rotation1 || reset bras 1*/
                if (servoToRot[0] == 1){
                    followupState = State::rotate1;
                    servoToRot[0] = 0;}
                /* déplace les bras pour rotation2*/
                else if (servoToRot[1] == 1){
                    servoToMove[0] = 0; servoToMove[1] = 40; servoToMove[2] = 180; servoToMove[3] = 180;
                    followupState = State::rotate2;
                    servoToRot[1] = 0;}
                /* déplace les bras pour rotation3*/
                else if (servoToRot[2] == 1){
                    servoToMove[0] = 0; servoToMove[1] = 0; servoToMove[2] = 40; servoToMove[3] = 180;
                    followupState = State::rotate3;
                    servoToRot[2] = 0;}
                /* déplace les bras pour rotation4*/
                else if (servoToRot[3] == 1){
                    servoToMove[0] = 0; servoToMove[1] = 0; servoToMove[2] = 0; servoToMove[3] = 180;
                    followupState = State::rotate4;
                    servoToRot[3] = 0;}
                /* si plus aucun  kapla a retourner*/
                    else {
                    LOG_DEBUG("retourne à l'état init");
                    servoToMove[0] = 90; servoToMove[1] = 90; servoToMove[2] = 90; servoToMove[3] = 90;
                    finished = 1;}
                if(finished){currentState = State::init;}
                else{currentState = State::movement;}
                break;}

            case State::rotate1:{
                LOG_DEBUG("retourne le kapla 1, regular status: ", regular);
                arduino->servoMove(servo::rotation1, 180*regular);
                T_start =  millis();
                servoCurrentRot[0] = regular;
                currentState = State::rotwait;
                break;}

            case State::rotate2:{
                LOG_DEBUG("retourne le kapla 2, regular status: ", regular);
                arduino->servoMove(servo::rotation2, 180*regular);
                T_start =  millis();
                servoCurrentRot[1] = regular;
                currentState = State::rotwait;
                break;}

            case State::rotate3:{
                LOG_DEBUG("retourne le kapla 3, regular status: ", regular);
                arduino->servoMove(servo::rotation3, 180*regular);
                T_start =  millis();
                servoCurrentRot[2] = regular;
                currentState = State::rotwait;
                break;}

            case State::rotate4:{
                LOG_DEBUG("retourne le kapla 4, regular status: ", regular);
                arduino->servoMove(servo::rotation4, 180*regular);
                T_start =  millis();
                servoCurrentRot[3] = regular;
                currentState = State::rotwait;
                break;}

            /*case mouvement : déplacement des bras 1 puis 2, 3, 4 (déplacement uniquement si nécessaire)*/
            case State::movement:{
                /* bouge le servo 1 si nécessaire*/
                if(servoToMove[0] != servoCurrentPos[0]){
                    LOG_DEBUG("bouge servo 1 à ", servoToMove[0]);
                    arduino->servoMove(servo::slider1, servoToMove[0]);
                    servoCurrentPos[0] = servoToMove[0];
                    T_start =  millis();
                    currentState = State::wait;
                }
                /* bouge le servo 2 si nécessaire*/
                else if(servoToMove[1] != servoCurrentPos[1]){
                    LOG_DEBUG("bouge servo 2 à ", servoToMove[1]);
                    arduino->servoMove(servo::slider2, servoToMove[1]);
                    servoCurrentPos[1] = servoToMove[1];
                    T_start =  millis();
                    currentState = State::wait;
                }
                /* bouge le servo 3 si nécessaire*/
                else if(servoToMove[2] != servoCurrentPos[2]){
                    LOG_DEBUG("bouge servo 3 à ", servoToMove[2]);
                    arduino->servoMove(servo::slider3, servoToMove[2]);
                    servoCurrentPos[2] = servoToMove[2];
                    T_start =  millis();
                    currentState = State::wait;
                }
                /* bouge le servo 4 si nécessaire*/
                else if(servoToMove[3] != servoCurrentPos[3]){
                    LOG_DEBUG("bouge servo 4 à ", servoToMove[3]);
                    arduino->servoMove(servo::slider4, servoToMove[3]);
                    servoCurrentPos[3] = servoToMove[3];
                    T_start =  millis();
                    currentState = State::wait;
                }
                else{
                    currentState = followupState;
                }
                break;}
            /*case wait : attente liée au déplacement d'un bras */
            case State::wait:{
                int fs = millis() - T_start;
                if(fs > TIMESLIDER){
                    LOG_DEBUG("wait ", TIMESLIDER, " ms");
                    if (toCopy || finished){currentState = State::init;}
                    else{currentState = State::movement;}
                }
            break;}
            /*case rotwait : attente liée à la rotation d'un bras*/
            case State::rotwait:{
                int fs = millis() - T_start;
                if(fs > TIMEROTATE){
                    LOG_DEBUG("wait ", TIMEROTATE, " ms");
                    currentState = State::toProcess;
                }
            break;}
            /*case init : uniquement au début et à la fin (commence par le bras n°4, puis 3, 2, 1)*/
            case State::init:{
                /*On commence par le servo 4, puis 3 ,2 et 1*/
                if(servoToMove[3] != servoCurrentPos[3]){
                    LOG_DEBUG("bouge servo 4 à ", servoToMove[3]);
                    arduino->servoMove(servo::slider4, servoToMove[3]);
                    servoCurrentPos[3] = servoToMove[3];
                    T_start =  millis();
                    currentState = State::wait;
                }
                else if(servoToMove[2] != servoCurrentPos[2]){
                    LOG_DEBUG("bouge servo 3 à ", servoToMove[2]);
                    arduino->servoMove(servo::slider3, servoToMove[2]);
                    servoCurrentPos[2] = servoToMove[2];
                    T_start =  millis();
                    currentState = State::wait;
                }
                else if(servoToMove[1] != servoCurrentPos[1]){
                    LOG_DEBUG("bouge servo 2 à ", servoToMove[1]);
                    arduino->servoMove(servo::slider2, servoToMove[1]);
                    servoCurrentPos[1] = servoToMove[1];
                    T_start =  millis();
                    currentState = State::wait;
                }
                else if(servoToMove[0] != servoCurrentPos[0]){
                    LOG_DEBUG("bouge servo 1 à ", servoToMove[0]);
                    arduino->servoMove(servo::slider1, servoToMove[0]);
                    servoCurrentPos[0] = servoToMove[0];
                    T_start =  millis();
                    currentState = State::wait;
                }
                else if(finished){
                    LOG_DEBUG("---Tri fini---");
                    currentState = State::idle;
                    /*finished = 1*/
                    return(finished);
                }
                else {currentState = State::toProcess;}
            break;}
    }

    return (finished);   /* sortie de la boucle (= 0)*/
}


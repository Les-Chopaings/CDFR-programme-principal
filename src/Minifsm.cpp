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

            // case State::etape1:{
            //     /*logger a inclure*/
            //     int fs = millis() - T_start;
            //     if (fs > 999){
            //         startSeconds++;
            //         LOG_DEBUG("étape 1, secondes écoulées = ", startSeconds);
            //         currentState = State::etape2;
            //         return (0);}
            //     break;}

            default:{
                /*Etat idle, vérification des kapla à retourner*/
                toReset = 0;
                LOG_DEBUG("idle");
                LOG_DEBUG("verification de la liste initiale= ", rotate[0], rotate[1], rotate[2], rotate[3]);
                if (rotate[0] == 1 || rotate[1] == 1 || rotate[2] == 1 || rotate[3] == 1){
                    toCopy = 1;
                    servoToMove[0] = 0; servoToMove[1] = 180; servoToMove[2] = 180; servoToMove[3] = 180;
                    currentState = State::init;}
                break;}

            case State::toProcess:{
                /* Des kaplas doivent être retournés, on copie la liste des kaplas */
                LOG_DEBUG("toProcess");
                if (toCopy){
                    for (int i = 0; i <= 4; i++){
                        rotCopied[i] = rotate[i];}
                    toCopy = 0;}

                // vérification des kaplas à retourner
                LOG_DEBUG("verification de la liste en attente = ", rotCopied[0], rotCopied[1], rotCopied[2], rotCopied[3]);
                /* retourne le servo 1 si nécessaire*/
                if (rotCopied[0] == 1){
                    followupState = State::rotate1;
                    rotCopied[0] = 0;}
                /* retourne le servo 2 si nécessaire*/
                else if (rotCopied[1] == 1){
                    servoToMove[0] = 0; servoToMove[1] = 40; servoToMove[2] = 180; servoToMove[3] = 180;
                    followupState = State::rotate2;
                    rotCopied[1] = 0;}
                /* retourne le servo 3 si nécessaire*/
                else if (rotCopied[2] == 1){
                    servoToMove[0] = 0; servoToMove[1] = 0; servoToMove[2] = 40; servoToMove[3] = 180;
                    followupState = State::rotate3;
                    rotCopied[2] = 0;}
                /* retourne le servo 4 si nécessaire*/
                else if (rotCopied[3] == 1){
                    servoToMove[0] = 0; servoToMove[1] = 0; servoToMove[2] = 0; servoToMove[3] = 180;
                    followupState = State::rotate4;
                    rotCopied[3] = 0;}
                else {
                    LOG_DEBUG("retourne à l'état init");
                    servoToMove[0] = 90; servoToMove[1] = 90; servoToMove[2] = 90; servoToMove[3] = 90;
                    toReset = 1;}
                if(toReset){currentState = State::init;}
                else{currentState = State::movement;}
                break;}

            case State::rotate1:{
                LOG_DEBUG("retourne le kapla 1");
                arduino->servoMove(servo::rotation1,180);
                T_start =  millis();
                currentState = State::rotwait;
                break;}

            case State::rotate2:{
                LOG_DEBUG("retourne le kapla 2");
                arduino->servoMove(servo::rotation2,180);
                currentState = State::toProcess;
                T_start =  millis();
                currentState = State::rotwait;
                break;}

            case State::rotate3:{
                LOG_DEBUG("retourne le kapla 3");
                arduino->servoMove(servo::rotation3,180);
                currentState = State::toProcess;
                T_start =  millis();
                currentState = State::rotwait;
                break;}

            case State::rotate4:{
                LOG_DEBUG("retourne le kapla 4");
                arduino->servoMove(servo::rotation4,180);
                currentState = State::toProcess;
                T_start =  millis();
                currentState = State::rotwait;
                break;}

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

            case State::wait:{
                int fs = millis() - T_start;
                if(fs > TIMESLIDER){
                    LOG_DEBUG("wait ", TIMESLIDER, " ms");
                    if (toCopy || toReset){currentState = State::init;}
                    else{currentState = State::movement;}
                }
            break;}

            case State::rotwait:{
                int fs = millis() - T_start;
                if(fs > TIMEROTATE){
                    LOG_DEBUG("wait ", TIMEROTATE, " ms");
                    currentState = State::toProcess;
                }
            break;}

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
                else if(toReset){
                    LOG_DEBUG("---Tri fini---");
                    currentState = State::idle;
                    return(1);
                }
                else {currentState = State::toProcess;}
            break;}
    }

    return (0);   /* sortie de la boucle (=0)*/
}


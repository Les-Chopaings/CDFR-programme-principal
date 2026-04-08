#include "Minifsm.hpp"
using namespace std;

Minifsm::Minifsm(/* args */)
{
}

Minifsm::~Minifsm()
{
}


int Minifsm::TriNoisette(bool* rotate)
{
switch (currentState) {
    
            case State::etape1:{
                //T_start = millis();
                /*logger a inclure*/
                LOG_DEBUG("étape 1");
                currentState = State::etape2;
                break;}

            case State::etape3:{
                /*logger a inclure*/                
                LOG_DEBUG("étape 3");
                currentState = State::idle;
                break;}

            case State::etape2:{
                /*logger a inclure*/                
                LOG_DEBUG("étape 2");
                currentState = State::etape3;
                break;}

            default:{
                /*Etat idle*/
                //unsigned long fs = T_start - millis();
                LOG_DEBUG("idle");
                currentState = State::etape1;
                // if (fs > 2000){
                //     currentState = State::etape1;
                //     return (0);}
                break;}
    return (1);   /* sortie de idle (=1)*/

    }
}


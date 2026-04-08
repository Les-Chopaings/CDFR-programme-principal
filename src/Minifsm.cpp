#include "Minifsm.hpp"
#include <chrono>
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
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;

            case State::etape1:
                auto T_start = Time::now();
                /*logger a inclure*/
                LOG_DEBUG("étape 1");
                currentState = State::etape3;
                break;

            case State::etape3:
                /*logger a inclure*/                
                LOG_DEBUG("étape 3");
                currentState = State::etape2;
                break;

            case State::etape2:
                /*logger a inclure*/                
                LOG_DEBUG("étape 2");
                currentState = State::idle;
                break;

            default:
                /*Etat idle*/
                fsec fs = T_start - Time::now();
                LOG_DEBUG("idle");
                if (fs.count() > 2){
                    currentState = State::etape1;
                    return (0);}
                break;
        }
    return (1);   /* sortie de idle (=1)*/

}


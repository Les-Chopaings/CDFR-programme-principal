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
                LOG_DEBUG("idle");
                LOG_DEBUG("verification de la liste initiale= ", rotate[0], rotate[1], rotate[2], rotate[3]);
                toCopy = 1;
                currentState = State::toProcess;
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
                if (rotCopied[0] == 1){
                    currentState = State::rotate1;
                    rotCopied[0] = 0;}
                else if (rotCopied[1] == 1){
                    currentState = State::rotate2;
                    rotCopied[1] = 0;}
                else if (rotCopied[2] == 1){
                    currentState = State::rotate3;
                    rotCopied[2] = 0;}
                else if (rotCopied[3] == 1){
                    currentState = State::rotate4;
                    rotCopied[3] = 0;}
                else {
                    currentState = State::idle;
                    return(1);}

                T_start = millis();
                break;}

            case State::rotate1:{
                /*Retourne le kapla 1*/
                int fs = millis() - T_start;
                if (fs > 999){
                    startSeconds++;
                    LOG_DEBUG("étape 1, secondes écoulées = ", startSeconds);
                    currentState = State::toProcess;}
                break;}

            case State::rotate2:{
                /*Retourne le kapla 2*/
                int fs = millis() - T_start;
                if (fs > 999){
                    startSeconds++;
                    LOG_DEBUG("étape 2, secondes écoulées = ", startSeconds);
                    currentState = State::toProcess;}
                break;}

            case State::rotate3:{
                /*Retourne le kapla 3*/
                int fs = millis() - T_start;
                if (fs > 999){
                    startSeconds++;
                    LOG_DEBUG("étape 3, secondes écoulées = ", startSeconds);
                    currentState = State::toProcess;}
                break;}

            case State::rotate4:{
                /*Retourne le kapla 4*/
                int fs = millis() - T_start;
                if (fs > 999){
                    startSeconds++;
                    LOG_DEBUG("étape 4, secondes écoulées = ", startSeconds);
                    currentState = State::toProcess;}
                break;}
    }

    return (1);   /* sortie de la boucle (=1)*/
}


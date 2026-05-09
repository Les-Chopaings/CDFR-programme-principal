#include "lidarAnalize.hpp"

void getTableValid(lidarAnalize_t* data, int count);

void rotateLidarData(lidarAnalize_t* data, int count, int angle){
    for(int i = 0; i< count; i++){
        data[i].angle = std::fmod(data[i].angle + angle, 360.0);
        if (data[i].angle < 0)
            data[i].angle += 360.0;
    }
}

void convertAngularToAxial(lidarAnalize_t* data, int count, position_t position){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            data[i].x = data[i].dist*cos((data[i].angle+position.theta)*DEG_TO_RAD) + position.x;
            data[i].y = -data[i].dist*sin((data[i].angle+position.theta)*DEG_TO_RAD) + position.y;
        }
    }
    getTableValid(data,count);
}

void convertAxialToAngular(lidarAnalize_t* data, int count, position_t position)
{
    for(int i = 0; i < count; i++)
    {
        if(data[i].valid)
        {
            float dx = data[i].x - position.x;
            float dy = -(data[i].y - position.y);
            data[i].dist = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx) * RAD_TO_DEG;
            data[i].angle = angle - position.theta;

            while(data[i].angle < 0)
                data[i].angle += 360;

            while(data[i].angle >= 360)
                data[i].angle -= 360;
        }
    }
}

void placeVirtualEnemy(lidarAnalize_t* data, int count, position_t robotPosition, position_t ennemyPosition)
{
    // Carré de 10 cm = 100 mm
    const int side = 100;
    // 1 point tous les 10 mm = 1 cm
    const int step = 10;

    // Coin haut-gauche du carré
    int startX = ennemyPosition.x - side / 2;
    int startY = ennemyPosition.y - side / 2;

    int index = 0;

    // Bord haut
    for (int x = 0; x <= side && index < count; x += step)
    {
        data[index].x = startX + x;
        data[index].y = startY;
        data[index].valid = true;
        index++;
    }

    // Bord droit
    for (int y = step; y <= side && index < count; y += step)
    {
        data[index].x = startX + side;
        data[index].y = startY + y;
        data[index].valid = true;
        index++;
    }

    // Bord bas
    for (int x = side - step; x >= 0 && index < count; x -= step)
    {
        data[index].x = startX + x;
        data[index].y = startY + side;
        data[index].valid = true;
        index++;
    }

    // Bord gauche
    for (int y = side - step; y > 0 && index < count; y -= step)
    {
        data[index].x = startX;
        data[index].y = startY + y;
        data[index].valid = true;
        index++;
    }

    // fill all data with invalid data
    for(int i = index; i< count; i++){
        data[i].valid = false;
    }
    convertAxialToAngular(data, count, robotPosition);
    getTableValid(data,count);
}

void printLidarAxial(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        const char* charMessage = "          ";
        if(!data[i].valid){
            charMessage = "non Valide";
        }
        if(data[i].onTable){
            charMessage = "non Table ";
        }
        printf("%s theta : %lf \t x : %d \ty : %d\n",charMessage,data[i].angle,data[i].x,data[i].y);
    }
}

void printAngular(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            printf("theta: %03.2f \tDist: %08.2f\n",data[i].angle,data[i].dist);
        }
        else{
            printf("non valid\n");
        }
    }
}


void maxDistance(lidarAnalize_t* data, int count,int& maxX, int maxY){
    maxX = 0;
    maxY = 0;
    for(int i = 0; i< count; i++){
        if(abs(data[i].x) > maxX)
            maxX = abs(data[i].x);
        if(abs(data[i].y) > maxY)
            maxY = abs(data[i].y);
    }
}


bool collideFordward(lidarAnalize_t* data, int count){
    for(int i = 0; i <count; i++){
        if(data[i].valid && data[i].onTable)
            if(data[i].angle <45 || data[i].angle>(360-45))
                if(data[i].dist < 500){
                    return true;
                }
    }
    return false;
}

bool collideBackward(lidarAnalize_t* data, int count){
    for(int i = 0; i <count; i++){
        if(data[i].valid && data[i].onTable)
            if(data[i].angle<(180+45) && data[i].angle>(180-45))
                if(data[i].dist < 500){
                    return true;
                }
    }
    return false;
}

int getDirection(int distanceStop){
    static int PreviousDistanceStop  = 0;

    //keep direction
    if(distanceStop!=0){
        if(distanceStop >= 0){
            if(PreviousDistanceStop == -1){
                LOG_DEBUG("FORWARD");
            }
            PreviousDistanceStop = 1;
        }
        else{
            if(PreviousDistanceStop == 1){
                LOG_DEBUG("BACKWARD");
            }
            PreviousDistanceStop = -1 ;
        }
    }
    else{
        distanceStop = PreviousDistanceStop;
    }
    return PreviousDistanceStop;
}

int collide(lidarAnalize_t* data, int count ,int direction){
    #ifdef EMULATE
    {
        int iRet = 12000;
        for(int i = 0; i <count; i++){
            if(data[i].valid && data[i].onTable){
                if(direction >= 0){
                    if(data[i].dist < iRet){
                        iRet = data[i].dist;
                    }
                }
                else{
                    if(data[i].dist < iRet){
                        iRet = data[i].dist;
                    }
                }
            }
        }
        return iRet;
    }
    #endif

    int iRet = 12000; //maximum capation distance for lidar
    for(int i = 0; i <count; i++){
        if(data[i].valid && data[i].onTable){
            if(direction >= 0){
                if(data[i].angle <45 || data[i].angle>(360-45)){
                    if(data[i].dist < iRet){
                        iRet = data[i].dist;
                    }
                }
            }
            else{
                if(data[i].angle<(180+45) && data[i].angle>(180-45)){
                    if(data[i].dist < iRet){
                        iRet = data[i].dist;
                    }
                }
            }
        }
    }
    //LOG_DEBUG("Distance Stop : ",distanceStop);
    //LOG_DEBUG("Collide : ",iRet);
    // LOG_DEBUG("Prec : ",iRetPre);
    return iRet;
}

void getTableValid(lidarAnalize_t* data, int count){
    for(int i = 0; i <count; i++){
        if(data[i].valid){
            if(data[i].x>50 && data[i].x<2950 && data[i].y>50 && data[i].y<1950)
                data[i].onTable = true;
            else
                data[i].onTable = false;
        }
    }
}

void pixelArtPrint(lidarAnalize_t* data, int count,int realSizeX,int realSizeY,int scale,position_t position){

    int sizeX = realSizeX/scale;
    int sizeY = realSizeY/scale;
    char* matriceAffichage;
    matriceAffichage = (char*)malloc(sizeX * sizeY * sizeof(char));

    //initMatrice
    for(int i = 0; i<sizeX * sizeY; i++){
        matriceAffichage[i] = '.';
    }

    int posix, posiy;
    for(int i = 0; i<count; i++){
        if(data[i].valid == true){
            posix = data[i].x/scale;
            posiy = data[i].y/scale;
            if(posix<sizeX && posix>=0 && posiy<sizeY && posiy>=0){
                if(data[i].onTable)
                    matriceAffichage[posix + sizeX * posiy] = '*';
                else
                    matriceAffichage[posix + sizeX * posiy] = 'X';
            }
        }
    }

    //fill

    // int positionRoboty = position.x/scale + sizeX/2;
    // int positionRobotx = position.y/scale + sizeY/2;

    // for(int i = 0; i<sizeX; i++){
    //     char chartype = ' ';
    //     for(int j = positionRoboty; j<sizeY; j++){
    //         int posX = MAP(j,positionRoboty,sizeY,positionRobotx,i);
    //         if(matriceAffichage[posX + sizeX * j] != ' '){
    //             chartype = matriceAffichage[posX + sizeX * j];
    //         }
    //         if(chartype != ' '){
    //             matriceAffichage[posX + sizeX * j] = chartype;
    //         }
    //     }
    // }

    // for(int i = 0; i<sizeX; i++){
    //     char chartype = ' ';
    //     for(int j = positionRoboty; j>=0; j--){
    //         int posX = MAP(j,positionRoboty,0,positionRobotx,i);
    //         if(matriceAffichage[posX + sizeX * j] != ' '){
    //             chartype = matriceAffichage[posX + sizeX * j];
    //         }
    //         if(chartype != ' '){
    //             matriceAffichage[posX + sizeX * j] = chartype;
    //         }
    //     }
    // }


    // for(int j = 0; j<sizeY; j++){
    //     char chartype = ' ';
    //     for(int i = positionRobotx; i<sizeX; i++){
    //         int posY = MAP(i,positionRobotx,sizeX,positionRoboty,j);
    //         if(matriceAffichage[i + sizeX * posY] != ' '){
    //             chartype = matriceAffichage[i + sizeX * posY];
    //         }
    //         if(chartype != ' '){
    //             matriceAffichage[i + sizeX * posY] = chartype;
    //         }
    //     }
    // }

    // for(int j = 0; j<sizeY; j++){
    //     char chartype = ' ';
    //     for(int i = positionRobotx; i>=0; i--){
    //         int posY = MAP(i,positionRobotx,0,positionRoboty,j);
    //         if(matriceAffichage[i + sizeX * posY] != ' '){
    //             chartype = matriceAffichage[i + sizeX * posY];
    //         }
    //         if(chartype != ' '){
    //             matriceAffichage[i + sizeX * posY] = chartype;
    //         }
    //     }
    // }

    posix = position.x/scale;
    posiy = position.y/scale;
    if(posix<sizeX && posix>=0 && posiy<sizeY && posiy>=0)
        matriceAffichage[posix + sizeX * posiy] = 'O';

    //print
    for(int y = sizeY-1; y>=0; y--){
        for(int x = 0 ; x<sizeX; x++){
            printf("%c%c",matriceAffichage[x + y * sizeX],matriceAffichage[x + y * sizeX]);
        }
        printf("\n");
    }

    free(matriceAffichage);
}
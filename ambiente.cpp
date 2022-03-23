//
// Created by luciano on 12/03/2022.
//
#include "ambiente.h"

ambiente::ambiente() {
    map = nullptr;
    mapSize[0] = 0;
    mapSize[1] = 0;
}

ambiente::~ambiente() {
    for(int i=0; i<mapSize[0]; i++){
        free(map[i]);
    }
    free(map);

    for(int i=0; i<victims.size(); i++){
        free(victims[i]);
    }
}

void ambiente::setMap(int **pMap,int* size) {
    map = pMap;
    mapSize[0] = size[0];
    mapSize[1] = size[1];
}

void ambiente::includeVictim(float *victim) {
    victims.push_back(victim);
}

void ambiente::includeRobots(vasculhador *pVasc, socorrista *pSoc) {
    roboS = pSoc;
    roboV = pVasc;
}

void ambiente::printMap() {
    int poseSoc[2], poseVas[2];
    roboS->getPose(poseSoc);
    roboV->getPose(poseVas);

    for(int i = 0; i< mapSize[0]; i++){
        for(int j=0 ;j <mapSize[1] ; j++){
            cout  << "|\t" << map[i][j];
            if(i == poseSoc[0] && j == poseSoc[1]){
                cout<< '+';
            }
            if(i == poseVas[0] && j == poseVas[1]){
                cout<< '*';
            }
            cout  << "\t|";
        }
        cout << endl;
    }
}

void ambiente::printVictims() {
    for(int i=0; i<victims.size(); i++){
        cout << "vitima " << i << ": ";
        for (int j = 0; j < 9; j++) {
            cout << victims[i][j] << " ";
        }
        cout << endl;
    }
}

int ambiente::getMap(const int *coord) {

    if((coord[0] < 0 && coord[0] >= mapSize[0]) && (coord[1] < 0 && coord[1] >= mapSize[1])){
        return -1;
    }
    return map[coord[0]][coord[1]];
}

int ambiente::tryMoveVasc(int move) {
    int nextPose[2], poseVasculhador[2];
    roboV->getPose(poseVasculhador);
    switch (move) {
        case DOWN:
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1];
            break;
        case UP:
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1];
            break;
        case RIGHT:
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] + 1;
            break;
        case LEFT:
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] - 1;
            break;
        case UP_RIGHT:
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] + 1;
            break;
        case UP_LEFT:
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] - 1;
            break;
        case DOWN_RIGHT:
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] + 1;
            break;
        case DOWN_LEFT:
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] - 1;
            break;
        default:
            return -1;
    }
    if ((nextPose[0] >= 0) && (nextPose[1] >= 0) && (nextPose[0] < mapSize[0]) && (nextPose[1] < mapSize[1])) {
       /* if(map[nextPose[0]][nextPose[1]] == -1)
            return -1;
        else*/
       return map[nextPose[0]][nextPose[1]];
    }
    else
        return -1;
}

float* ambiente::getVictim(int linha, int coluna){
    float* victim;
    for(int i=0; i<victims.size(); i++){
       if(victims[i][0]==linha && victims[i][1]==coluna)
           victim = victims[i];
    }
    return victim;
}


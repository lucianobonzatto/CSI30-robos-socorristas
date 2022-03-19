//
// Created by luciano on 12/03/2022.
//
#include "ambiente.h"

ambiente::ambiente() {
    map = nullptr;
    mapSize[0] = 0;
    mapSize[1] = 0;
    poseSocorrista[0] = 0;
    poseSocorrista[1] = 0;
    poseVasculhador[0] = 0;
    poseVasculhador[1] = 0;
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

void ambiente::printMap() {
    for(int i = 0; i< mapSize[0]; i++){
        for(int j=0 ;j <mapSize[1] ; j++){
            cout  << "|\t" << map[i][j];
            if(i == poseSocorrista[0] && j == poseSocorrista[1]){
                cout<< '+';
            }
            if(i == poseVasculhador[0] && j == poseVasculhador[1]){
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

void ambiente::setPoseVasculhador(int *pose) {
    poseVasculhador[0] = pose[0];
    poseVasculhador[1] = pose[1];
}

void ambiente::setPoseSocorrista(int *pose) {
    poseSocorrista[0] = pose[0];
    poseSocorrista[1] = pose[1];
}

int ambiente::getMap(int *coord) {

    if((coord[0] < 0 && coord[0] >= mapSize[0]) && (coord[1] < 0 && coord[1] >= mapSize[1])){
        return -1;
    }
    return map[coord[0]][coord[1]];
}
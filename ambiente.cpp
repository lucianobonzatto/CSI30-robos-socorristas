//
// Created by lucia on 12/03/2022.
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

void ambiente::printMap() {
    for(int i = 0; i< mapSize[0]; i++){
        for(int j=0 ;j <mapSize[1] ; j++){
            cout << "|\t"<< map[i][j] << "\t|";
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
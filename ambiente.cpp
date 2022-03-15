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
}

void ambiente::setMap(int **pMap,int* size) {
    map = pMap;
    mapSize[0] = size[0];
    mapSize[1] = size[1];
}

void ambiente::printMap() {
    for(int i = 0; i< mapSize[0]; i++){
        for(int j=0 ;j <mapSize[1] ; j++){
            cout << "|\t"<< map[i][j] << "\t|";
        }
        cout << endl;
    }
}
//
// Created by lucia on 12/03/2022.
//
#include "principal.h"

using namespace std;

principal::principal() {
    int mapSize[2];
    mapSize[0] = 3;
    mapSize[1] = 4;
    ifstream configFile ( "../config.txt", ios::in );
    ifstream ambienteFile ( "../ambiente.txt", ios::in );


    int** mat = (int**) malloc(mapSize[0]*sizeof(int*));
    for (int i = 0; i < mapSize[0]; i++){
        mat[i] = (int*) malloc(mapSize[1] * sizeof(int));
    }
    for(int i = 0; i< mapSize[0]; i++){
        for(int j=0 ;j <mapSize[1] ; j++){
            mat[i][j] = i+j;
        }
    }
    map.setMap(mat, mapSize);
    //map.printMap();
}
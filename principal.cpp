//
// Created by lucia on 12/03/2022.
//
#include "principal.h"

using namespace std;

principal::principal() {
    initMap();
}


void principal::initMap() {
    int mapSize[2], objPose[2];
    int eixo = 0;
    mapSize[0] = 0;
    mapSize[1] = 0;
    ifstream configFile ( "../config.txt", ios::in );
    ifstream ambienteFile ( "../ambiente.txt", ios::in );
    string line;

    //read maxLin
    std::getline(configFile, line);
    for(int i=7; i<line.size(); i++){
        mapSize[0] *= 10;
        mapSize[0] += line[i] - '0';
    }
    //read maxCol
    std::getline(configFile, line);
    for(int i=7; i<line.size(); i++){
        mapSize[1] *= 10;
        mapSize[1] += line[i] - '0';
    }

    //creat the map
    int** mat = (int**) malloc(mapSize[0]*sizeof(int*));
    for (int i = 0; i < mapSize[0]; i++){
        mat[i] = (int*) malloc(mapSize[1] * sizeof(int));
        for (int j = 0; j < mapSize[1]; j++) {
            mat[i][j] = 0;
        }
    }

    //read walls
    std::getline(ambienteFile, line);
    for(int i=6; i<line.size(); i++){
        if(line[i] == ' '){
            readCoord(line, i+1, objPose);
            if ((objPose[0] >= 0) && (objPose[1] >= 0) && (objPose[0] < mapSize[0]) && (objPose[1] < mapSize[1])) {
                mat[objPose[0]][objPose[1]] = -1;
                //cout << "parade -> " << objPose[0] << ',' << objPose[1] << endl;
            }
            else {
                cout << "posicao de parede invalida:" << objPose[0] << ',' << objPose[1] << endl;
            }
        }
    }

    //read victims
    int numVictims = 0;
    float* victim;
    while (std::getline(ambienteFile, line)){
        int i = 7;
        i = readCoord(line, 7, objPose);
        numVictims++;
        victim = (float *) malloc(9*sizeof (float ));
        if ((objPose[0] >= 0) && (objPose[1] >= 0) && (objPose[0] < mapSize[0]) && (objPose[1] < mapSize[1])) {
            mat[objPose[0]][objPose[1]] = numVictims;
            victim[0] = objPose[0];
            victim[1] = objPose[1];
            //cout << "parade -> " << objPose[0] << ',' << objPose[1] << endl;
        }
        else {
            cout << "posicao de vitima invalida:" << objPose[0] << ',' << objPose[1] << endl;
            continue;
        }

        victim[2] = 0;
        int flg = 0;
        int j = 2;
        for(i++; i<line.size(); i++){
            if(line[i] == ' '){
                j++;
                victim[j] = 0;
                flg = 0;
            }
            else if(line[i] == '.'){
                flg = 10;
            }
            else {
                if(flg == 0){
                    victim[j] *= 10;
                    victim[j] += line[i] - '0';
                }
                else {
                    victim[j] += ((float)line[i] - '0')/flg;
                    flg *= 10;
                }
            }
        }
        map.includeVictim(victim);
    }

    map.setMap(mat, mapSize);
    map.printMap();
    map.printVictims();
}

int principal::readCoord(string line, int firsVal, int *pose) {
    int eixo = 0, i = -1;
    pose[0] = 0;
    pose[1] = 0;
    for(i=firsVal; i<line.size(); i++){
        if(line[i] == ','){
            eixo++;
        }
        else if(line[i] == ' ') {
            return i;
        }
        else{
            pose[eixo] *= 10;
            pose[eixo] += line[i] - '0';
        }
    }
    return i;
}
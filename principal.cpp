//
// Created by lucia on 12/03/2022.
//
#include "principal.h"

using namespace std;

principal::principal() {
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
    float victim[9];
    while (std::getline(ambienteFile, line)){
        int i = 7;
        i = readCoord(line, 7, objPose);
        numVictims++;
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

        for(int j = 2; j<9; j++){
            victim[j] = 0;
        }

        cout << "\nvitima " << numVictims << "-> ";
        for (int j=0; j<6; j++)
            cout << ' ' << victim[j];
        cout << endl;
    }

    map.setMap(mat, mapSize);
    map.printMap();
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
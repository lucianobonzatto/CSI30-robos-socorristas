//
// Created by luciano on 12/03/2022.
//
#include "principal.h"

using namespace std;

principal::principal() {
    tempoVasculhador = 0;
    tempoSocorrista = 0;
    capacidadeSocorrista = 0;

    bateriaVasculhador = 0;
    bateriaSocorrista = 0;
    initMap();
    initRobots();
    map.includeRobots(&roboV, &roboS);

    //map.printMap();
    //map.printVictims();
}

void principal::initRobots() {
    ifstream configFile ( "../config.txt", ios::in );
    string line;
    int mapSize[2] = {0,0};

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
    roboV.setMapSize(mapSize);
    roboS.setMapSize(mapSize);

    //read Tv
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        tempoVasculhador *= 10;
        tempoVasculhador += line[i] - '0';
    }
    roboV.setBat(tempoVasculhador);

    //read Ts
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        tempoSocorrista *= 10;
        tempoSocorrista += line[i] - '0';
    }
    roboS.setBat(tempoSocorrista);

    //read Bv
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        bateriaVasculhador *= 10;
        bateriaVasculhador += line[i] - '0';
    }
    roboV.setBat(bateriaVasculhador);

    //read Bs
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        bateriaSocorrista *= 10;
        bateriaSocorrista += line[i] - '0';
    }
    roboS.setBat(bateriaSocorrista);

    //read Ks
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        capacidadeSocorrista *= 10;
        capacidadeSocorrista += line[i] - '0';
    }
    roboS.setNumPacotes(capacidadeSocorrista);

    roboV.inicUntried();
}

void principal::initMap() {
    int mapSize[2], objPose[2];
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
        int i;
        i = readCoord(line, 7, objPose);
        numVictims++;
        victim = (float *) malloc(9*sizeof (float ));
        if ((objPose[0] >= 0) && (objPose[1] >= 0) && (objPose[0] < mapSize[0]) && (objPose[1] < mapSize[1])) {
            mat[objPose[0]][objPose[1]] = numVictims;
            victim[0] = (float) objPose[0];
            victim[1] = (float) objPose[1];
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
                    victim[j] *= (float) 10;
                    victim[j] += (float) (line[i] - '0');
                }
                else {
                    victim[j] += ((float)line[i] - '0')/(float)flg;
                    flg *= 10;
                }
            }
        }
        map.includeVictim(victim);
    }

    map.setMap(mat, mapSize);
}

int principal::readCoord(string line, int firsVal, int *pose) {
    int eixo = 0, i;
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

void principal::ciclo() {
    int move = DOWN, result;
    int nextPose[2];
    while(tempoVasculhador >= 0){
        cout << "tempo: " << tempoVasculhador << endl;
        map.printMap();

        move = roboV.moveDecision();
        result = tratMoveVasculhador(move, nextPose);

        roboV.moveResult(result, nextPose, tempoVasculhador);
        cout << "\t" << move << " -> " << result << endl;

        /*switch (move) {
            case DOWN:
                move = RIGHT;
                break;
            case RIGHT:
                move = UP;
                break;
            case UP:
                move = LEFT;
                break;
            case LEFT:
                move = DOWN;
                break;
        }*/
    }
}

int principal::tratMoveVasculhador(int move, int nextPose[2]) {
    int poseVasculhador[2];
    roboV.getPose(poseVasculhador);
    roboV.getPose(nextPose);
    switch (move) {
        case DOWN:
            tempoVasculhador--;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1];
            return 1;
        case UP:
            tempoVasculhador--;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1];
            return 1;
        case RIGHT:
            tempoVasculhador--;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] + 1;
            return 1;
        case LEFT:
            tempoVasculhador--;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] - 1;
            return 1;
        case UP_RIGHT:
            tempoVasculhador = tempoVasculhador - 1.5;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] + 1;
            return 1;
        case UP_LEFT:
            tempoVasculhador = tempoVasculhador - 1.5;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] - 1;
            return 1;
        case DOWN_RIGHT:
            tempoVasculhador = tempoVasculhador - 1.5;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] + 1;
            return 1;
        case DOWN_LEFT:
            tempoVasculhador = tempoVasculhador - 1.5;
            if (map.tryMoveVasc(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] - 1;
            return 1;
        default:
            tempoVasculhador--;
            return -1;
    }
}
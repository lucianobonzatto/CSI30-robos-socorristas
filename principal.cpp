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
    roboV.setTime(tempoVasculhador);

    //read Ts
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        tempoSocorrista *= 10;
        tempoSocorrista += line[i] - '0';
    }
    roboS.setTime(tempoSocorrista);

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
    int move = DOWN, result=0;
    int nextPose[2];

    while(tempoVasculhador > 0){
       // cout << "tempo: " << tempoVasculhador << endl;
        //cout << "\t" << move << " -> " << result << endl;
    //    map.printMap();
        move = roboV.moveDecision();
        result = tratMoveVasculhador(move, nextPose);
        if(bateriaVasculhador<=0 || tempoVasculhador <=0){
            break;
        }
        roboV.moveResult(result, nextPose, tempoVasculhador, bateriaVasculhador);
        map.printMap();
        cout<< "tempo " <<tempoVasculhador << " | bateria " << bateriaVasculhador<< endl ;
    }
    //cout << "====================================================================================" << endl;
    //roboV.printMap();
    //roboV.printVictims();

    /*int vascPose[2], socPose[2];
    if(vascPose[0] == socPose[0] && vascPose[1] == socPose[1]){
        roboV.shareVictims(&roboS);
        roboV.shareMap(&roboS);
    }*/

    //cout << "====================================================================================" << endl;
    //roboS.printMap();
    //roboS.printVictims();

    /*while(tempoSocorrista > 0){
        cout << "====================================================================================" << endl;
        map.printMap();
        move = roboS.moveDecision();
        result = tratMoveSocorrista(move, nextPose);
        roboS.moveResult(result, nextPose, tempoSocorrista, bateriaSocorrista);
    }*/
}

int principal::tratMoveVasculhador(int move, int nextPose[2]) {
    int poseVasculhador[2];
    float* victim;
    roboV.getPose(poseVasculhador);
    roboV.getPose(nextPose);
    switch (move) {
        case READ_VICTIM:
            tempoVasculhador -= 2;
            bateriaVasculhador -= 2;
            victim = map.getVictim(poseVasculhador[0],poseVasculhador[1]);
            roboV.includeVictim(victim);
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1];
            return 1;
        case DOWN:
            tempoVasculhador--;
            bateriaVasculhador--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1];
            return map.tryMoveVasculhador(move);
        case UP:
            tempoVasculhador--;
            bateriaVasculhador--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1];
            return map.tryMoveVasculhador(move);
        case RIGHT:
            tempoVasculhador--;
            bateriaVasculhador--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] + 1;
            return map.tryMoveVasculhador(move);
        case LEFT:
            tempoVasculhador--;
            bateriaVasculhador--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] - 1;
            return map.tryMoveVasculhador(move);
        case UP_RIGHT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhador = bateriaVasculhador - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] + 1;
            return map.tryMoveVasculhador(move);
        case UP_LEFT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhador = bateriaVasculhador - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] - 1;
            return map.tryMoveVasculhador(move);
        case DOWN_RIGHT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhador = bateriaVasculhador - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] + 1;
            return map.tryMoveVasculhador(move);
        case DOWN_LEFT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhador = bateriaVasculhador - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] - 1;
            return map.tryMoveVasculhador(move);
        default:
            tempoVasculhador--;
            bateriaVasculhador--;
            return -1;
    }
}

int principal::tratMoveSocorrista(int move, int *nextPose) {
    int poseSocorrista[2];
    float* victim;
    roboS.getPose(poseSocorrista);
    roboS.getPose(nextPose);
    switch (move) {
        case READ_VICTIM:
            tempoSocorrista -= 2;
            bateriaSocorrista -= 2;
            victim = map.getVictim(poseSocorrista[0],poseSocorrista[1]);
            roboV.includeVictim(victim);
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1];
            return 1;
        case DOWN:
            tempoSocorrista--;
            bateriaSocorrista--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1];
            return map.tryMoveSocorrista(move);
        case UP:
            tempoSocorrista--;
            bateriaSocorrista--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1];
            return map.tryMoveSocorrista(move);
        case RIGHT:
            tempoSocorrista--;
            bateriaSocorrista--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1] + 1;
            return map.tryMoveSocorrista(move);
        case LEFT:
            tempoSocorrista--;
            bateriaSocorrista--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1] - 1;
            return map.tryMoveSocorrista(move);
        case UP_RIGHT:
            tempoSocorrista -= 1.5;
            bateriaSocorrista -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1] + 1;
            return map.tryMoveSocorrista(move);
        case UP_LEFT:
            tempoSocorrista -= 1.5;
            bateriaSocorrista -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1] - 1;
            return map.tryMoveSocorrista(move);
        case DOWN_RIGHT:
            tempoSocorrista -= 1.5;
            bateriaSocorrista -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1] + 1;
            return map.tryMoveSocorrista(move);
        case DOWN_LEFT:
            tempoSocorrista -= 1.5;
            bateriaSocorrista -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1] - 1;
            return map.tryMoveSocorrista(move);
        default:
            tempoSocorrista--;
            bateriaSocorrista--;
            return -1;
    }
}
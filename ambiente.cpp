//
// autores: Luciano Bonzatto Junior e Juliana Martins Ferreira
// professor: Cesar Augusto Tacla
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
        for (int j = 0; j < 15; j++) {
            cout << victims[i][j] << " ";
        }
        cout << endl;
    }
}

void ambiente::printVictimasSalvas() {
    for(int i=0; i<victimasSalvas.size(); i++){
        cout << "vitima salva" << i << ": ";
        for (int j = 0; j < 15; j++) {
            cout << victimasSalvas[i][j] << " ";
        }
        cout << endl;
    }
}

void ambiente::printVictimasSalvasGrav() {
    double limMax = 1, limMin = 0.9;
    int num = 0;
    for (int i = 0; i < 10; i++) {
        cout << "]" << limMin << ", " << limMax << "] -> ";
        num = 0;

        for(int i=0; i<victimasSalvas.size(); i++){
            if(victimasSalvas[i][7] > limMin && victimasSalvas[i][7] <= limMax){
                num++;
            }
        }
        limMax = limMin;
        limMin -= 0.1;
        cout << num << endl;
    }
}

int ambiente::getMap(const int *coord) {

    if((coord[0] < 0 && coord[0] >= mapSize[0]) && (coord[1] < 0 && coord[1] >= mapSize[1])){
        return -1;
    }
    return map[coord[0]][coord[1]];
}

int ambiente::getNumVitimas() {
    return victims.size();
}

int ambiente::getNumVitimasSalvas() {
    return victimasSalvas.size();
}

int ambiente::socorristaSoltouKit() {
    int pose[2];
    roboS->getPose(pose);

    if(map[pose[0]][pose[1]] > 0){
        for(int i=0; i<victims.size(); i++){
            if(victims[i][0] == pose[0] && victims[i][1] == pose[1]){
                victimasSalvas.push_back( victims[i] );
                map[pose[0]][pose[1]] = 0;
                return 1;
            }
        }
    }

    return 0;
}

int ambiente::tryMoveVasculhador(int move) {
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

int ambiente::tryMoveSocorrista(int move) {
    int nextPose[2], poseSocorrista[2];
    roboS->getPose(poseSocorrista);
    switch (move) {
        case DOWN:
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1];
            break;
        case UP:
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1];
            break;
        case RIGHT:
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1] + 1;
            break;
        case LEFT:
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1] - 1;
            break;
        case UP_RIGHT:
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1] + 1;
            break;
        case UP_LEFT:
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1] - 1;
            break;
        case DOWN_RIGHT:
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1] + 1;
            break;
        case DOWN_LEFT:
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1] - 1;
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

void ambiente::shareMap(socorrista *roboS) {
    roboS->includeMap(map);
}

void ambiente::shareVictims(socorrista *roboS) {
    for (int i = 0; i < victims.size(); i++) {
        roboS->includeVictim(victims[i]);
    }
}

vector<float*> ambiente::getVictims(){
    return victims;
}
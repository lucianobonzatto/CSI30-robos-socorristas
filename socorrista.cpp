//
// Created by luciano on 18/03/2022.
//

#include "socorrista.h"

socorrista::socorrista() {
    pose[0] = 0;
    pose[1] = 0;
    cargaBateriaAtual = 0;
    tempoRestante = 0;
    numPacotes = 0;
    mapa = nullptr;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
}

socorrista::~socorrista() {

}

void socorrista::initMat() {

}

int socorrista::moveDecision() {
    return 0;
}

void socorrista::moveResult(int result, const int *newPose, float time, float bat) {
    pose[0] = newPose[0];
    pose[1] = newPose[1];
    tempoRestante = time;
    cargaBateriaAtual = bat;
}

void socorrista::setMapSize(int mapSize[2]) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void socorrista::setBat(float bat) {
    cargaBateriaAtual = bat;
}

void socorrista::setTime(float time){
    tempoRestante = time;
}

void socorrista::setNumPacotes(int num) {
    numPacotes = num;
}

void socorrista::includeMap(int** pMap){
    mapa = pMap;
}

void socorrista::includeVictim(float* victim){
    victimsV.push_back(victim);
}

void socorrista::getPose(int *poseReturn) {
    poseReturn[0] = pose[0];
    poseReturn[1] = pose[1];
}

void socorrista::printMap(){
    for(int i = 0; i< tamAmbiente[0]; i++){
        for(int j=0 ;j <tamAmbiente[1] ; j++){
            cout  << "|\t" << mapa[i][j];
            if(i == pose[0] && j == pose[1])
                cout << "*";
            cout  << "\t|";
        }
        cout << endl;
    }
}

void socorrista::printVictims(){
    for(int i=0; i<victimsV.size(); i++){
        cout << "vitima " << i << ": ";
        for (int j = 0; j < 9; j++) {
            cout << victimsV[i][j] << " ";
        }
        cout << endl;
    }
}
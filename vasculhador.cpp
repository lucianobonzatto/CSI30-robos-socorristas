//
// Created by luciano on 12/03/2022.
//

#include "vasculhador.h"

vasculhador::vasculhador() {
    pose[0] = 0;
    pose[1] = 0;
    cargaBateriaAtual = 0;
    mapa = nullptr;

    proxMovimento = 0;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
    tempoRestante = 0;

}

void vasculhador::setMapSize(int mapSize[2]) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void vasculhador::setBat(int bat) {
    cargaBateriaAtual = bat;
}

void vasculhador::setTime(int time){
    tempoRestante = time;
}

void  vasculhador::getPose(int *poseReturn) {
    poseReturn[0] = pose[0];
    poseReturn[1] = pose[1];
}

void vasculhador::inicUntried(){
    untried = (int***) malloc(tamAmbiente[0]*sizeof(int**));
    for (int i = 0; i < tamAmbiente[0]; i++){
        untried[i] = (int**) malloc(tamAmbiente[1] * sizeof(int*));
        for (int j = 0; j < tamAmbiente[1]; j++) {
            untried[i][j] = (int*) malloc(8 * sizeof(int));
            for (int k = 0; k < 8; k++)
                untried[i][j][k] = 0;
        }
    }
}

int vasculhador::moveDecision() {
    proxMovimento = -1;
    for (int i=0;i<8;i++){
        if (untried[pose[0]][pose[1]][i] == 0){
            proxMovimento = i;
            break;
        }
    }
    if(proxMovimento == -1){
        // procurar no mapa
    }
    return proxMovimento;
}

void vasculhador::moveResult(int result, int *newPose, int time) {
    pose[0] = newPose[0];
    pose[1] = newPose[1];
    tempoRestante = time;
}
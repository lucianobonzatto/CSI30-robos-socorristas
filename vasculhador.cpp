//
// Created by luciano on 12/03/2022.
//

#include "vasculhador.h"

vasculhador::vasculhador() {
    pose[0] = 0;
    pose[1] = 0;
    cargaBateriaAtual = 0;
    mapa = (int**) malloc(tamAmbiente[0]*sizeof(int*));
    for (int i = 0; i < tamAmbiente[0]; i++){
        mapa[i] = (int*) malloc(tamAmbiente[1] * sizeof(int));
        for (int j = 0; j < tamAmbiente[1]; j++) {
            mapa[i][j] = -2; // Não foi visitado
        }
    }
    untried = nullptr;

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
            untried[pose[0]][pose[1]][i] = 1;
            break;
        }
    }
    if(proxMovimento == -1){ //Caso já tenha explorado as 8 vizinhaças, se desloca para a primeira que não tem parede
        if(mapa[pose[0]][pose[1] + 1] != -1){
            proxMovimento = RIGHT;
        } else if (mapa[pose[0] + 1][pose[1]] != -1){
            proxMovimento = DOWN;
        } else if (mapa[pose[0]][pose[1] - 1] != -1) {
            proxMovimento = LEFT;
        } else if (mapa[pose[0] - 1][pose[1]] != -1) {
            proxMovimento = UP;
        }  else if (mapa[pose[0] + 1][pose[1] + 1] != -1) {
            proxMovimento = DOWN_RIGHT;
        }  else if (mapa[pose[0] + 1][pose[1] - 1] != -1) {
            proxMovimento = DOWN_LEFT;
        } else if (mapa[pose[0] - 1][pose[1] + 1] != -1) {
            proxMovimento = UP_RIGHT;
        } else if (mapa[pose[0] - 1][pose[1] - 1] != -1) {
            proxMovimento = UP_LEFT;
        }
    }
    return proxMovimento;
}

void vasculhador::moveResult(int result, int *newPose, int time) {
    pose[0] = newPose[0];
    pose[1] = newPose[1];
    tempoRestante = time;

    if (result == 0){ //Caso o movimento não tenha sido realizado, há uma parede
        switch (proxMovimento) {
            case DOWN:
                mapa[pose[0] + 1][pose[1]] = -1;
                break;
            case UP:
                mapa[pose[0] - 1][pose[1]] = -1;
                break;
            case RIGHT:
                mapa[pose[0]][pose[1] + 1] = -1;
                break;
            case LEFT:
                mapa[pose[0]][pose[1] - 1] = -1;
                break;
            case UP_RIGHT:
                mapa[pose[0] - 1][pose[1] + 1] = -1;
                break;
            case DOWN_RIGHT:
                mapa[pose[0] + 1][pose[1] + 1] = -1;
                break;
            case UP_LEFT:
                mapa[pose[0] - 1][pose[1] - 1] = -1;
                break;
            case DOWN_LEFT:
                mapa[pose[0] + 1][pose[1] - 1] = -1;
                break;
            default:
                break;
        }
    }
    else{ // Marca que posição foi visitada
        switch (proxMovimento) {
            case DOWN:
                mapa[pose[0] + 1][pose[1]] = 0;
                break;
            case UP:
                mapa[pose[0] - 1][pose[1]] = 0;
                break;
            case RIGHT:
                mapa[pose[0]][pose[1] + 1] = 0;
                break;
            case LEFT:
                mapa[pose[0]][pose[1] - 1] = 0;
                break;
            case UP_RIGHT:
                mapa[pose[0] - 1][pose[1] + 1] = 0;
                break;
            case DOWN_RIGHT:
                mapa[pose[0] + 1][pose[1] + 1] = 0;
                break;
            case UP_LEFT:
                mapa[pose[0] - 1][pose[1] - 1] = 0;
                break;
            case DOWN_LEFT:
                mapa[pose[0] + 1][pose[1] - 1] = 0;
                break;
            default:
                break;
        }
    }
}
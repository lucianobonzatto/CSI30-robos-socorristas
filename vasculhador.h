//
// Created by luciano on 12/03/2022.
//

#ifndef TAREFA_1_VASCULHADOR_H
#include <iostream>
#include <vector>
#include "includes.h"
using namespace std;
#define TAREFA_1_VASCULHADOR_H


class vasculhador {
private:
    int pose[2];
    int cargaBateriaAtual;
    int tempoRestante;
    int** mapa;
    int*** untried;
    vector<float*> victimsV;

    int proxMovimento;

    //constantes
    int tamAmbiente[2];
public:
    vasculhador();
    ~vasculhador();

    int moveDecision();
    void moveResult(int result, int newPose[2], int time);

    void setMapSize(int mapSize[2]);
    void setBat(int bat);
    void setTime(int time);
    void inicUntried();

    void getPose(int* poseReturn);

    void includeVictim(float* victim);
};

#endif //TAREFA_1_VASCULHADOR_H

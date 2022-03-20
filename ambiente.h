//
// Created by luciano on 12/03/2022.
//

#ifndef TAREFA_1_AMBIENTE_H
#include <iostream>
#include <vector>

#include "vasculhador.h"
#include "socorrista.h"

using namespace std;
#define TAREFA_1_AMBIENTE_H

class ambiente {
private:
    vasculhador* roboV;
    socorrista* roboS;
    int** map;
    int mapSize[2];
    vector<float*> victims;

public:
    ambiente();
    ~ambiente();

    void includeVictim(float* victim);
    void includeRobots(vasculhador* pVasc, socorrista* pSoc);

    void printMap();
    void printVictims();

    void setMap(int** pMap, int* size);
    int getMap(int coord[2]);

    int tryMoveVasc(int move);
};

#endif //TAREFA_1_AMBIENTE_H

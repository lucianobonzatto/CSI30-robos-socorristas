//
// Created by luciano on 12/03/2022.
//

#ifndef TAREFA_1_AMBIENTE_H
#include <iostream>
#include <vector>
using namespace std;
#define TAREFA_1_AMBIENTE_H

class ambiente {
private:
    int poseSocorrista[2];
    int poseVasculhador[2];
    int** map;
    int mapSize[2];
    vector<float*> victims;

public:
    ambiente();
    ~ambiente();

    void includeVictim(float* victim);

    void printMap();
    void printVictims();

    void setMap(int** pMap, int* size);
    void setPoseSocorrista(int* pose);
    void setPoseVasculhador(int* pose);

    int getMap(int coord[2]);
};

#endif //TAREFA_1_AMBIENTE_H

//
// Created by lucia on 12/03/2022.
//

#ifndef TAREFA_1_AMBIENTE_H
#include <iostream>
#include <vector>
using namespace std;
#define TAREFA_1_AMBIENTE_H

class ambiente {
private:
    int** map;
    int mapSize[2];
    vector<float*> victims;

public:
    ambiente();
    ~ambiente();

    void setMap(int** pMap, int* size);
    void includeVictim(float* victim);
    void printMap();
    void printVictims();
};

#endif //TAREFA_1_AMBIENTE_H

//
// Created by luciano on 18/03/2022.
//

#ifndef TAREFA_1_SOCORRISTA_H
#include <iostream>
#include <vector>
#include <list>

#include "includes.h"

using namespace std;
#define TAREFA_1_SOCORRISTA_H

class socorrista {
private:
    int pose[2]{};
    float cargaBateriaAtual;
    int numPacotes;
    float tempoRestante;
    int** mapa;
    vector<float*> victimsV;
    list<point> caminho;

    // busca off-line
    float*** costs;

    //constantes
    int tamAmbiente[2]{};
    
    //metodos
    int buscaUniforme(const int *partida, const int *objetivo);
    void readCaminho(const int *partida, const int *objetivo);
public:
    socorrista();
    ~socorrista();
    
    int moveDecision();
    void moveResult(int result, const int newPose[2], float time, float bat);
    
    void setMapSize(int mapSize[2]);
    void setBat(float bat);
    void setTime(float time);
    void setNumPacotes(int num);

    void initMat();
    void includeMap(int** pMap);
    void includeVictim(float* victim);
    
    void getPose(int* poseReturn);
    void printMap();
    void printVictims();
    void printCaminho();
};

#endif //TAREFA_1_SOCORRISTA_H

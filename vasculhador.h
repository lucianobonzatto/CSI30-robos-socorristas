//
// Created by luciano on 12/03/2022.
//

#ifndef TAREFA_1_VASCULHADOR_H
#include <iostream>
#include <vector>
#include <list>

#include "includes.h"
#include "socorrista.h"
using namespace std;
#define TAREFA_1_VASCULHADOR_H


class vasculhador {
private:
    int pose[2]{};
    float  cargaBateriaAtual;
    float tempoRestante;
    int** mapa;
    int proxMovimento;
    bool recarregar;
    bool returnPos;
    point ultPose;

    // busca off-line
    float*** costs;

    // busca on-line
    int*** untried;
    vector<float*> victimsV;
    list<point> caminho;

    //constantes
    int tamAmbiente[2]{};

    //metodos
    float buscaUniforme(const int objetivo[2]);
    void readCaminho(const int *objetivo);

public:
    vasculhador();
    ~vasculhador();

    int moveDecision();
    void moveResult(int result, const int newPose[2], float time, float bat);

    void setMapSize(const int mapSize[2]);
    void setBat(float bat);
    void setTime(float time);
    void inicUntried();
    void inicCoats();

    void getPose(int* poseReturn);
    float getBat();

    void includeVictim(float* victim);
    void printVictims();
    void printMap();
    void printCaminho();

    void shareVictims(socorrista* roboS);
    void shareMap(socorrista* roboS);
};

#endif //TAREFA_1_VASCULHADOR_H

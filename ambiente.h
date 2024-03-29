//
// autores: Luciano Bonzatto Junior e Juliana Martins Ferreira
// professor: Cesar Augusto Tacla
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
    vector<float*> victimasSalvas;

public:
    ambiente();
    ~ambiente();

    float* getVictim(int linha, int coluna);
    vector<float*> getVictims();

    void includeVictim(float* victim);
    void includeRobots(vasculhador* pVasc, socorrista* pSoc);

    void printMap();
    void printVictims();
    void printVictimasSalvas();
    void printVictimasSalvasGrav();

    void setMap(int** pMap, int* size);
    int getMap(const int coord[2]);
    int getNumVitimas();
    int getNumVitimasSalvas();

    int socorristaSoltouKit();

    int tryMoveVasculhador(int move);
    int tryMoveSocorrista(int move);

    void shareVictims(socorrista* roboS);
    void shareMap(socorrista* roboS);
};

#endif //TAREFA_1_AMBIENTE_H

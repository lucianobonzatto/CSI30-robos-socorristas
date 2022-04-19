//
// autores: Luciano Bonzatto Junior e Juliana Martins Ferreira
// professor: Cesar Augusto Tacla
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
    int proxMovimento;
    float tempoRestante;
    int** mapa;
    vector<float*> victimsV;
    list<point> caminho;
    int state;
    int numVitimasSel;

    // busca off-line
    float*** costs;

    // algoritmo genético
    vector<int*> population;

    //constantes
    int tamAmbiente[2]{};
    int numPacotesMax;
    int numGen;
    int numCrossover;
    int probCrossover;
    int probMutation;
    int chromossomeSize;
    
    //metodos
    int buscaUniforme(const int *partida, const int *objetivo);
    void readCaminho(const int *partida, const int *objetivo);
    vector<int*> crossover(vector<int*> popSelec);
    vector<int*> mutation(vector<int*> popCross);
    void includePopulation(vector<int*> popMutation);
    int fit(int *vet);
    void createFirstGen();

public:
    socorrista();
    ~socorrista();
    
    int moveDecision();
    void moveResult(int result, const int newPose[2], float time, float bat);
    
    void setMapSize(int mapSize[2]);
    void setBat(float bat);
    void setTime(float time);
    void setNumPacotesMax(int num);
    void incNumPacotes();
    int decNumPacotes();

    void initMat();
    void includeMap(int** pMap);
    void includeVictim(float* victim);
    
    void getPose(int* poseReturn);
    void printMap();
    void printVictims();
    void printCaminho();
    void printPopulation();
};

#endif //TAREFA_1_SOCORRISTA_H

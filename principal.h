//
// autores: Luciano Bonzatto Junior e Juliana Martins Ferreira
// professor: Cesar Augusto Tacla
//

#ifndef TAREFA_1_PRINCIPAL_H
#include <fstream>
#include <string>
#include "includes.h"

#include "ambiente.h"
#include "vasculhador.h"
#include "socorrista.h"
#define TAREFA_1_PRINCIPAL_H

class principal {
private:
    ambiente map;
    vasculhador roboV;
    socorrista roboS;
    float bateriaVasculhador;
    float bateriaSocorrista;
    float bateriaVasculhadorAtual;
    float bateriaSocorristaAtual;

    float tempoVasculhador;
    float tempoSocorrista;
    float tempoVasculhadorTotal;
    float tempoSocorristaTotal;

    int capacidadeSocorrista;

    void initRobots();
    void initMap();

    int readCoord(string line, int firsVal, int *pose);
    int tratMoveVasculhador(int move, int nextPose[2]);
    int tratMoveSocorrista(int move, int nextPose[2]);
public:
    principal();
    void ciclo();
};

#endif //TAREFA_1_PRINCIPAL_H

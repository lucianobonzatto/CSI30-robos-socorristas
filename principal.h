//
// Created by luciano on 12/03/2022.
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
    int bateriaVasculhador;
    int bateriaSocorrista;

    int tempoVasculhador;
    int tempoSocorrista;

    int capacidadeSocorrista;

    void initRobots();
    void initMap();

    int readCoord(string line, int firsVal, int *pose);
    int tratMoveVasculhador(int move, int nextPose[2]);
public:
    principal();
    void ciclo();
};

#endif //TAREFA_1_PRINCIPAL_H

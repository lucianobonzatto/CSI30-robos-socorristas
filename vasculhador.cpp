//
// Created by lucia on 12/03/2022.
//

#include "vasculhador.h"

vasculhador::vasculhador() {
    pose[0] = 0;
    pose[1] = 0;
    cargaAtual = 0;
    mapa = nullptr;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
    cargaInicial = 0;
}

void vasculhador::setMapSize(int mapSize[2]) {
    pose[0] = mapSize[0];
    pose[1] = mapSize[1];
}

void vasculhador::setCargaInicial(int bat) {
    cargaInicial = bat;
}
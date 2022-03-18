//
// Created by luciano on 12/03/2022.
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
    cargaBateriaInicial = 0;
}

void vasculhador::setMapSize(int mapSize[2]) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void vasculhador::setCargaInicial(int bat) {
    cargaBateriaInicial = bat;
}
//
// Created by lucia on 18/03/2022.
//

#include "socorrista.h"

socorrista::socorrista() {
    pose[0] = 0;
    pose[1] = 0;
    cargaAtual = 0;
    mapa = nullptr;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
    cargaInicial = 0;
}

void socorrista::setMapSize(int mapSize[2]) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void socorrista::setCargaInicial(int bat) {
    cargaInicial = bat;
}
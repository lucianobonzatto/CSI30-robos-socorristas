//
// Created by luciano on 18/03/2022.
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
    cargaBateriaInicial = 0;
    capacidadPacote = 0;
}

void socorrista::setMapSize(int mapSize[2]) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void socorrista::setCargaInicial(int bat) {
    cargaBateriaInicial = bat;
}

void socorrista::setCapacidadPacote(int carga){
    capacidadPacote = carga;
}
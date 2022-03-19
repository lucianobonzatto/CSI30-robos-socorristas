//
// Created by luciano on 18/03/2022.
//

#include "socorrista.h"

socorrista::socorrista() {
    pose[0] = 0;
    pose[1] = 0;
    cargaBateriaAtual = 0;
    mapa = nullptr;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
}

void socorrista::setMapSize(int mapSize[2]) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void socorrista::setBat(int bat) {
    cargaBateriaAtual = bat;
}

void socorrista::setTime(int time){
    tempoRestante = time;
}

void socorrista::setNumPacotes(int num) {
    numPacotes = num;
}
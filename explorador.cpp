//
// Created by lucia on 12/03/2022.
//

#include "explorador.h"

explorador::explorador() {
    pose[0] = 0;
    pose[1] = 0;
    carga_bateria = 0;
    mapa = nullptr;

    //constantes
    tam_ambiente[0] = 0;
    tam_ambiente[1] = 0;
    carga_bateria_inicial = 0;
}
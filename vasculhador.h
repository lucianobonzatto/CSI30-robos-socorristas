//
// Created by luciano on 12/03/2022.
//

#ifndef TAREFA_1_VASCULHADOR_H
#define TAREFA_1_VASCULHADOR_H


class vasculhador {
private:
    int pose[2];
    int cargaAtual;
    int* mapa;

    //constantes
    int tamAmbiente[2];
    int cargaBateriaInicial;
public:
    vasculhador();
    void setMapSize(int mapSize[2]);
    void setCargaInicial(int bat);
};

#endif //TAREFA_1_VASCULHADOR_H

//
// Created by luciano on 18/03/2022.
//

#ifndef TAREFA_1_SOCORRISTA_H
#define TAREFA_1_SOCORRISTA_H


class socorrista {
private:
    int pose[2];
    int cargaAtual;
    int* mapa;

    //constantes
    int tamAmbiente[2];
    int cargaBateriaInicial;
    int capacidadPacote;
public:
    socorrista();
    void setMapSize(int mapSize[2]);
    void setCargaInicial(int bat);
    void setCapacidadPacote(int carga);
};


#endif //TAREFA_1_SOCORRISTA_H

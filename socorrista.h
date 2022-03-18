//
// Created by lucia on 18/03/2022.
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
    int cargaInicial;
public:
    socorrista();
    void setMapSize(int mapSize[2]);
    void setCargaInicial(int bat);

};


#endif //TAREFA_1_SOCORRISTA_H

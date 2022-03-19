//
// Created by luciano on 18/03/2022.
//

#ifndef TAREFA_1_SOCORRISTA_H
#define TAREFA_1_SOCORRISTA_H


class socorrista {
private:
    int pose[2];
    int cargaBateriaAtual;
    int numPacotes;
    int tempoRestante;
    int* mapa;

    //constantes
    int tamAmbiente[2];
public:
    socorrista();
    void setMapSize(int mapSize[2]);
    void setBat(int bat);
    void setTime(int time);
    void setNumPacotes(int num);
};

#endif //TAREFA_1_SOCORRISTA_H

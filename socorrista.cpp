//
// Created by luciano on 18/03/2022.
//

#include "socorrista.h"

socorrista::socorrista() {
    pose[0] = 0;
    pose[1] = 0;
    cargaBateriaAtual = 0;
    tempoRestante = 0;
    numPacotes = 0;
    mapa = nullptr;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
}

socorrista::~socorrista() {

}

void socorrista::initMat() {
    costs = (float***) malloc(tamAmbiente[0]*sizeof(float**));
    for (int i = 0; i < tamAmbiente[0]; i++){
        costs[i] = (float**) malloc(tamAmbiente[1] * sizeof(float*));
        for (int j = 0; j < tamAmbiente[1]; j++) {
            costs[i][j] = (float*) malloc(4 * sizeof(float));
            costs[i][j][0] = -1;
            costs[i][j][1] = -1;
            costs[i][j][2] = -1;
            costs[i][j][3] = -1;
        }
    }
}

int socorrista::moveDecision() {
    return RIGHT;
}

void socorrista::moveResult(int result, const int *newPose, float time, float bat) {
    pose[0] = newPose[0];
    pose[1] = newPose[1];
    tempoRestante = time;
    cargaBateriaAtual = bat;
}

void socorrista::setMapSize(int mapSize[2]) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void socorrista::setBat(float bat) {
    cargaBateriaAtual = bat;
}

void socorrista::setTime(float time){
    tempoRestante = time;
}

void socorrista::setNumPacotes(int num) {
    numPacotes = num;
}

void socorrista::includeMap(int** pMap){
    mapa = pMap;
}

void socorrista::includeVictim(float* victim){
    victimsV.push_back(victim);
}

void socorrista::getPose(int *poseReturn) {
    poseReturn[0] = pose[0];
    poseReturn[1] = pose[1];
}

void socorrista::printMap(){
    for(int i = 0; i< tamAmbiente[0]; i++){
        for(int j=0 ;j <tamAmbiente[1] ; j++){
            cout  << "|\t" << mapa[i][j];
            if(i == pose[0] && j == pose[1])
                cout << "+";
            cout  << "\t|";
        }
        cout << endl;
    }
}

void socorrista::printVictims(){
    for(int i=0; i<victimsV.size(); i++){
        cout << "vitima " << i << ": ";
        for (int j = 0; j < 9; j++) {
            cout << victimsV[i][j] << " ";
        }
        cout << endl;
    }
}

void socorrista::printCaminho() {
    cout << "| ";
    for (std::list<point>::iterator it=caminho.begin(); it != caminho.end(); ++it){
        cout << it->x << ", " << it->y << " | ";
    }
    cout << endl;
}

int socorrista::buscaUniforme(const int *partida, const int *objetivo) {
    list<point> vizinhanca;
    point atual, aux;
    float custo;

    for (int i = 0; i < tamAmbiente[0]; i++){
        for (int j = 0; j < tamAmbiente[1]; j++) {
            costs[i][j][0] = -1;
            costs[i][j][1] = -1;
            costs[i][j][2] = -1;
            costs[i][j][3] = -1;
        }
    }

    costs[partida[0]][partida[1]][0] = (float) partida[0];
    costs[partida[0]][partida[1]][1] = (float) partida[1];
    costs[partida[0]][partida[1]][2] = 0;

    atual.x = partida[0];
    atual.y = partida[1];
    vizinhanca.push_back(atual);

    while(true){
        if(vizinhanca.empty())
            return -1;
        else{
            atual = vizinhanca.front();
            vizinhanca.pop_front();
            costs[atual.x][atual.y][3] = 1;

//            cout << "\tatual ->" << atual.x << "," << atual.y;
//            cout << "\t\ttamAmbiente ->" << tamAmbiente[0] << "," << tamAmbiente[1];
//            cout << "\t\partida ->" << partida[0] << "," << partida[1] << endl;
//            cout << "\t\tobjetivo ->" << objetivo[0] << "," << objetivo[1] << endl;

            if(atual.x == objetivo[0] && atual.y == objetivo[1]) {
                return costs[objetivo[0]][objetivo[1]][2];
            }
            else{
                for(int i=0; i<8; i++){
                    switch (i) {
                        case 0:
                            aux.x = atual.x+1;
                            aux.y = atual.y;
                            custo = 1;
                            break;
                        case 1:
                            aux.x = atual.x-1;
                            aux.y = atual.y;
                            custo = 1;
                            break;
                        case 2:
                            aux.x = atual.x;
                            aux.y = atual.y+1;
                            custo = 1;
                            break;
                        case 3:
                            aux.x = atual.x;
                            aux.y = atual.y-1;
                            custo = 1;
                            break;
                        case 4:
                            aux.x = atual.x+1;
                            aux.y = atual.y-1;
                            custo = 1.5;
                            break;
                        case 5:
                            aux.x = atual.x+1;
                            aux.y = atual.y+1;
                            custo = 1.5;
                            break;
                        case 6:
                            aux.x = atual.x-1;
                            aux.y = atual.y+1;
                            custo = 1.5;
                            break;
                        case 7:
                            aux.x = atual.x-1;
                            aux.y = atual.y-1;
                            custo = 1.5;
                            break;
                    }

                    //                   cout  << endl << "\taux " << i << "-> " << aux.x << "," << aux.y;
                    if((aux.x < 0) || (aux.x >= tamAmbiente[0]) || (aux.y < 0) || (aux.y >= tamAmbiente[1])){
//                        cout << " fora";
                    }
                    else if(mapa[aux.x][aux.y] == -1 || mapa[aux.x][aux.y] == -2){
//                        cout << " parede";
                    }
                    else{
//                        cout << " entrou" << endl;
                        custo = custo + costs[atual.x][atual.y][2];
//                        cout << "\t\t" << aux.x << "," << aux.y << " _ " << costs[aux.x][aux.y][2] << " _ " << custo;
                        if((costs[aux.x][aux.y][3] == 1) && (costs[aux.x][aux.y][2] < custo)){
                            continue;
                        }

                        if(costs[aux.x][aux.y][2] == -1 || costs[aux.x][aux.y][2] > custo){
                            costs[aux.x][aux.y][0] = atual.x;
                            costs[aux.x][aux.y][1] = atual.y;
                            costs[aux.x][aux.y][2] = custo;
                            costs[aux.x][aux.y][3] = -1;
                        }
                        else{
                            continue;
                        }

                        if(vizinhanca.empty()){
                            vizinhanca.push_back(aux);
                            continue;
                        }

                        //verifica se ja não esta na lista, se sim remove
                        for (std::list<point>::iterator it=vizinhanca.begin(); it != vizinhanca.end(); ++it){
                            if(aux.x == it->x && aux.y == it->y){
                                vizinhanca.erase(it);
                            }
                        }

                        //inclui na posição correta para manter a ordenação
                        for (std::list<point>::iterator it=vizinhanca.begin(); it != vizinhanca.end(); ++it){
                            if(costs[it->x][it->y][2] > custo){
                                vizinhanca.insert(it, aux);
                                continue;
                            }
                        }
                        vizinhanca.push_back(aux);
                    }
                }
            }
        }
    }
}

void socorrista::readCaminho(const int *partida, const int *objetivo) {
    point atual, prox;

    atual.x = objetivo[0];
    atual.y = objetivo[1];

    if(!caminho.empty())
        return;
    while ((atual.x != partida[0]) || (atual.y != partida[1])){
        caminho.push_front(atual);
        prox.x = costs[atual.x][atual.y][0];
        prox.y = costs[atual.x][atual.y][1];

        atual.x = prox.x;
        atual.y = prox.y;
    }
}
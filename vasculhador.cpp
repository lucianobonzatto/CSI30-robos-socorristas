//
// Created by luciano on 12/03/2022.
//

#include "vasculhador.h"

vasculhador::vasculhador() {
    pose[0] = 0;
    pose[1] = 0;
    cargaBateriaAtual = 0;
    mapa = nullptr;
    untried = nullptr;
    costs = nullptr;
    recarregar = false;
    returnPos = false;

    proxMovimento = 0;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
    tempoRestante = 0;
}

vasculhador::~vasculhador() {
    if (mapa != nullptr)
    {
        for (int i = 0; i < tamAmbiente[0]; i++){
            free(mapa[i]);
        }
        free(mapa);
    }

    for (int i = 0; i < tamAmbiente[0]; i++){
        for (int j = 0; j < tamAmbiente[1]; j++) {
            free(untried[i][j]);
        }
        free(untried[i]);
    }
    free(untried);

    for (int i = 0; i < tamAmbiente[0]; i++){
        for (int j = 0; j < tamAmbiente[1]; j++) {
            free(costs[i][j]);
        }
        free(costs[i]);
    }
    free(costs);
}

void vasculhador::setMapSize(const int* mapSize) {
    tamAmbiente[0] = mapSize[0];
    tamAmbiente[1] = mapSize[1];
}

void vasculhador::setBat(float  bat) {
    cargaBateriaAtual = bat;
}

void vasculhador::setTime(float time){
    tempoRestante = time;
}

void  vasculhador::getPose(int *poseReturn) {
    poseReturn[0] = pose[0];
    poseReturn[1] = pose[1];
}

float vasculhador::getBat() {
    return cargaBateriaAtual;
}

void vasculhador::inicCoats() {
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
void vasculhador::inicUntried(){
    mapa = (int**) malloc(tamAmbiente[0]*sizeof(int*));
    for (int i = 0; i < tamAmbiente[0]; i++){
        mapa[i] = (int*) malloc(tamAmbiente[1] * sizeof(int));
        for (int j = 0; j < tamAmbiente[1]; j++) {
            mapa[i][j] = -2; // Não foi visitado
        }
    }
    mapa[0][0] = 0;

    untried = (int***) malloc(tamAmbiente[0]*sizeof(int**));
    for (int i = 0; i < tamAmbiente[0]; i++){
        untried[i] = (int**) malloc(tamAmbiente[1] * sizeof(int*));
        for (int j = 0; j < tamAmbiente[1]; j++) {
            untried[i][j] = (int*) malloc(8 * sizeof(int));
            for (int k = 0; k < 8; k++){
                untried[i][j][k] = 0;
            }
            if (i==0){ //Primeira linha
                untried[0][j][3] = 1; //Não pode ir para cima
                untried[0][j][6] = 1; //Não pode ir para diagonal Right-up
                untried[0][j][7] = 1; //Não pode ir para diagonal left-up
            } else if (i==tamAmbiente[0] - 1){ //Ultima linha
                untried[tamAmbiente[0] - 1][j][1] = 1; //Não pode ir para baixo
                untried[tamAmbiente[0] - 1][j][4] = 1; //Não pode ir para diagonal Right-down
                untried[tamAmbiente[0] - 1][j][5] = 1; //Não pode ir para diagonal left-down
            }
            if (j==0){ //Primeira coluna
                untried[i][0][2] = 1; //Não pode ir para esquerda
                untried[i][0][5] = 1; //Não pode ir para diagonal left-down
                untried[i][0][7] = 1; //Não pode ir para diagonal left-up
            } else if (j== tamAmbiente[1] - 1){ //ultima coluna
                untried[i][tamAmbiente[1] - 1][0] = 1; //Não pode ir para direita
                untried[i][tamAmbiente[1] - 1][4] = 1; //Não pode ir para diagonal right-down
                untried[i][tamAmbiente[1] - 1][6] = 1; //Não pode ir para diagonal right-up
            }
        }
    }
}

int vasculhador::moveDecision() {
    proxMovimento = -1;
    point nextPose;
    int incluida = 0;
    int obj[2] = {0,0};

    if(cargaBateriaAtual < 10 && pose[0] == 0 && pose[1] == 0){
        proxMovimento = RECARREGAR;
        return proxMovimento;
    }

    if (pose[0]==ultPose.x && pose[1]==ultPose.y){
        returnPos = false;
    }

    if(returnPos){
        obj[0] = ultPose.x;
        obj[1] = ultPose.y;
        inicCoats();
        buscaUniforme(obj);
        readCaminho(obj);
        // printCaminho();
        nextPose = caminho.front();
        caminho.pop_front();

        if ((nextPose.x == pose[0]) && (nextPose.y == (pose[1] + 1))){
            proxMovimento = RIGHT;
        } else if ((nextPose.x == pose[0]) && (nextPose.y == (pose[1] - 1))) {
            proxMovimento = LEFT;
        } else if ((nextPose.x == (pose[0] + 1)) && (nextPose.y == pose[1])) {
            proxMovimento = DOWN;
        } else if ((nextPose.x == (pose[0] - 1)) && (nextPose.y == pose[1])){
            proxMovimento = UP;
        } else if ((nextPose.x == (pose[0] + 1)) && (nextPose.y == (pose[1] + 1))) {
            proxMovimento = DOWN_RIGHT;
        } else if ((nextPose.x == (pose[0] + 1)) && (nextPose.y == (pose[1] - 1))) {
            proxMovimento = DOWN_LEFT;
        } else if ((nextPose.x == (pose[0] - 1)) && (nextPose.y == (pose[1] + 1))) {
            proxMovimento = UP_RIGHT;
        } else if ((nextPose.x == (pose[0] - 1)) && (nextPose.y == (pose[1] - 1))) {
            proxMovimento = UP_LEFT;
        }
        return  proxMovimento;
    }

    if(mapa[pose[0]][pose[1]] > 0){
        for (int i=0;i<victimsV.size();i++){
            if (victimsV[i][0] == pose[0] && victimsV[i][1] == pose[1]){
                incluida = 1;
                break;
            }
        }
        if(!incluida){
            proxMovimento = READ_VICTIM;
            return proxMovimento;
        }
    }

    inicCoats();
    float custo = buscaUniforme(obj);
    if(custo > cargaBateriaAtual - 5 || custo > tempoRestante - 5) { //5 = 1.5 + 2 + 1.5 -> maior custo para ir e voltar para o mesmo lugar
        recarregar = true;
       // cout << "Preciso retornar" << endl;
    }

    if(!recarregar){
        for (int i=0;i<8;i++){
            if (untried[pose[0]][pose[1]][i] == 0){
                proxMovimento = i;
                untried[pose[0]][pose[1]][i] = 1;
                break;
            }
        }
        if(proxMovimento == -1){ //Caso já tenha explorado as 8 vizinhaças, se desloca para a primeira que não tem parede
            if(mapa[pose[0]][pose[1] + 1] != -1){
                proxMovimento = RIGHT;
            } else if (mapa[pose[0] + 1][pose[1]] != -1){
                proxMovimento = DOWN;
            } else if (mapa[pose[0]][pose[1] - 1] != -1) {
                proxMovimento = LEFT;
            } else if (mapa[pose[0] - 1][pose[1]] != -1) {
                proxMovimento = UP;
            }  else if (mapa[pose[0] + 1][pose[1] + 1] != -1) {
                proxMovimento = DOWN_RIGHT;
            }  else if (mapa[pose[0] + 1][pose[1] - 1] != -1) {
                proxMovimento = DOWN_LEFT;
            } else if (mapa[pose[0] - 1][pose[1] + 1] != -1) {
                proxMovimento = UP_RIGHT;
            } else if (mapa[pose[0] - 1][pose[1] - 1] != -1) {
                proxMovimento = UP_LEFT;
            }
        }
    } else {
        readCaminho(obj);
       // printCaminho();
        nextPose = caminho.front();
        caminho.pop_front();

        if ((nextPose.x == pose[0]) && (nextPose.y == (pose[1] + 1))){
            proxMovimento = RIGHT;
        } else if ((nextPose.x == pose[0]) && (nextPose.y == (pose[1] - 1))) {
            proxMovimento = LEFT;
        } else if ((nextPose.x == (pose[0] + 1)) && (nextPose.y == pose[1])) {
            proxMovimento = DOWN;
        } else if ((nextPose.x == (pose[0] - 1)) && (nextPose.y == pose[1])){
            proxMovimento = UP;
        } else if ((nextPose.x == (pose[0] + 1)) && (nextPose.y == (pose[1] + 1))) {
            proxMovimento = DOWN_RIGHT;
        } else if ((nextPose.x == (pose[0] + 1)) && (nextPose.y == (pose[1] - 1))) {
            proxMovimento = DOWN_LEFT;
        } else if ((nextPose.x == (pose[0] - 1)) && (nextPose.y == (pose[1] + 1))) {
            proxMovimento = UP_RIGHT;
        } else if ((nextPose.x == (pose[0] - 1)) && (nextPose.y == (pose[1] - 1))) {
            proxMovimento = UP_LEFT;
        }
    }

    return proxMovimento;
}

void vasculhador::moveResult(int result, const int *newPose, float time, float bat) {
    if (result == -3){
        cout<<"Recarreguei"<<endl;
        returnPos = true;
        recarregar = false;
    }
    else {
        switch (proxMovimento) {
            case DOWN:
                mapa[pose[0] + 1][pose[1]] = result;
                break;
            case UP:
                mapa[pose[0] - 1][pose[1]] = result;
                break;
            case RIGHT:
                mapa[pose[0]][pose[1] + 1] = result;
                break;
            case LEFT:
                mapa[pose[0]][pose[1] - 1] = result;
                break;
            case UP_RIGHT:
                mapa[pose[0] - 1][pose[1] + 1] = result;
                break;
            case DOWN_RIGHT:
                mapa[pose[0] + 1][pose[1] + 1] = result;
                break;
            case UP_LEFT:
                mapa[pose[0] - 1][pose[1] - 1] = result;
                break;
            case DOWN_LEFT:
                mapa[pose[0] + 1][pose[1] - 1] = result;
                break;
            default:
                break;
        }
    }

    pose[0] = newPose[0];
    pose[1] = newPose[1];
    tempoRestante = time;
    cargaBateriaAtual = bat;


    /*printMap();
    cout << endl << endl;
    int obj[2] = {0,0};
    buscaUniforme(obj);

    for(int i = 0; i< tamAmbiente[0]; i++){
        for(int j=0 ;j <tamAmbiente[1] ; j++){
            cout  << "|\t" << costs[i][j][0] << "," << costs[i][j][1] << " _ " << costs[i][j][2] << "\t|";
        }
        cout << endl;
    }
    cout << "====================================================================================" << endl << endl;*/
}

void vasculhador::includeVictim(float *victim) {
    victimsV.push_back(victim);
}

void vasculhador::printVictims() {
    for(int i=0; i<victimsV.size(); i++){
        cout << "vitima " << i << ": ";
        for (int j = 0; j < 9; j++) {
            cout << victimsV[i][j] << " ";
        }
        cout << endl;
    }
}

void vasculhador::printMap() {
    for(int i = 0; i< tamAmbiente[0]; i++){
        for(int j=0 ;j <tamAmbiente[1] ; j++){
            cout  << "|\t" << mapa[i][j];
            if(i == pose[0] && j == pose[1])
                cout << "*";
            cout  << "\t|";
        }
        cout << endl;
    }
}

void vasculhador::printCaminho() {
    cout << "| ";
    for (std::list<point>::iterator it=caminho.begin(); it != caminho.end(); ++it){
        cout << it->x << ", " << it->y << " | ";
    }
    cout << endl;
}

float vasculhador::buscaUniforme(const int *objetivo) {
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

    costs[pose[0]][pose[1]][0] = (float) pose[0];
    costs[pose[0]][pose[1]][1] = (float) pose[1];
    costs[pose[0]][pose[1]][2] = 0;

    atual.x = pose[0];
    atual.y = pose[1];
    vizinhanca.push_back(atual);

    while(true){
 /*       cout << "fila: ";
        for (std::list<point>::iterator it=vizinhanca.begin(); it != vizinhanca.end(); ++it){
            cout << "|" << it->x << "," << it->y << " _ " << costs[it->x][it->y][2];
        }
        cout << "|" << endl;*/

        if(vizinhanca.empty())
            return -1;
        else{
            atual = vizinhanca.front();
            vizinhanca.pop_front();
            costs[atual.x][atual.y][3] = 1;

//            cout << "\tatual ->" << atual.x << "," << atual.y;
//            cout << "\t\ttamAmbiente ->" << tamAmbiente[0] << "," << tamAmbiente[1];
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

void vasculhador::shareVictims(socorrista *roboS) {
    for (int i = 0; i < victimsV.size(); i++) {
        roboS->includeVictim(victimsV[i]);
    }
}

void vasculhador::shareMap(socorrista *roboS) {
    roboS->includeMap(mapa);
}

void vasculhador::readCaminho(const int *objetivo) {
    point atual,prox;
    atual.x = objetivo[0];
    atual.y = objetivo[1];

    if(!caminho.empty())
        return;
    while ((atual.x != pose[0]) || (atual.y != pose[1])){
        caminho.push_front(atual);
        prox.x = costs[atual.x][atual.y][0];
        prox.y = costs[atual.x][atual.y][1];

        atual.x = prox.x;
        atual.y = prox.y;
    }
    if (recarregar){
        ultPose.x = atual.x;
        ultPose.y = atual.y;
    }
}
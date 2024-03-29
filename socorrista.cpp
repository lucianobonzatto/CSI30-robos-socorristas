//
// autores: Luciano Bonzatto Junior e Juliana Martins Ferreira
// professor: Cesar Augusto Tacla
//

#include "socorrista.h"

socorrista::socorrista() {
    pose[0] = 0;
    pose[1] = 0;
    cargaBateriaAtual = 0;
    tempoRestante = 0;
    numPacotes = 0;
    mapa = nullptr;
    state = PREPARANDO_ROTA;
    numVitimasSel = 0;

    //constantes
    tamAmbiente[0] = 0;
    tamAmbiente[1] = 0;
    numPacotesMax = 0;

    numGen = 100;
    numCrossover = 50;
    probCrossover = 80;
    probMutation = 5;

    srand(time(NULL));
}

socorrista::~socorrista() {
    for (int i = 0; i < tamAmbiente[0]; i++){
        for (int j = 0; j < tamAmbiente[1]; j++) {
            free(costs[i][j]);
        }
        free(costs[i]);
    }
    free(costs);

    for(int i=0; i<population.size(); i++){
        free(population[i]);
    }

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
    vector<int*> popSelec;
    vector<int*> popCross;
    vector<int*> popMutation;
    point nextPose;
    int partida[2], objetivo[2];
    int custo, custoTotal = 0, pesoTotal, cont, peso;
    int *chromossome;
    partida[0] = pose[0];
    partida[1] = pose[1];

    if(victimsV.empty()){
        return FINALIZAR_BUSCA;
    }

    if(state == PREPARANDO_ROTA){
        //Decidir vitimas
        chromossomeSize = victimsV.size()+1;
        createFirstGen();       //inclui cromossomos aleatorios validos
        //printPopulation();
        for(int gener = 0; gener < numGen; gener++){
            popSelec.clear();
            popCross.clear();
            popMutation.clear();

            //seleciona os primeiros cromossomos (que são os melhores)
            pesoTotal = 0;
            for(int i=0; i< population.size(); i++){
                pesoTotal += population[i][chromossomeSize-1];
            }
            while(popSelec.size() < numCrossover){
                peso = rand()%pesoTotal + 1;
                cont = population[0][chromossomeSize-1];
                int j = 0;
                while(cont < peso){
                    j++;
                    cont += population[j][chromossomeSize-1];
                }
                popSelec.push_back(population[j]);
            }

            popCross = crossover(popSelec);
            popMutation = mutation(popCross);
            includePopulation(popMutation);
        }

        numVitimasSel = population[0][chromossomeSize-1];
        //cout << numGen << "\n" << numCrossover << "\n" << probCrossover << "\n" << probMutation << endl;
        return FINALIZAR_BUSCA;
        /*numVitimasSel = 0;
        for(int i=0; i<victimsV.size(); i++){
            objetivo[0] = victimsV[i][0];
            objetivo[1] = victimsV[i][1];
            custo = buscaUniforme(partida, objetivo) + 0.5;

            custoTotal += custo;
            if(custoTotal < tempoRestante && custoTotal < cargaBateriaAtual){
                numVitimasSel++;
                readCaminho(partida, objetivo);
            }
            else{
                custoTotal -= custo;
                break;
            }

            if (numVitimasSel == numPacotesMax)
                break;

            partida[0] = objetivo[0];
            partida[1] = objetivo[1];
        }
        state = PEGANDO_KITS;*/
    }

    if(state == PEGANDO_KITS){
        //cout << "\testado -> " << state << "\tnum pacotes ->" << numPacotes <<endl;
        if(numPacotes < numVitimasSel){
            return CARREGAR_KIT;
        }
        state = SALVANDO_VITIMAS;
    }

    if(state == SALVANDO_VITIMAS){
        //cout << "\testado -> " << state << "\tnum pacotes ->" << numPacotes <<endl;
        if(pose[0] == victimsV[0][0] && pose[1] == victimsV[0][1]){
            proxMovimento = SOLTAR_KIT;
            return proxMovimento;
        }

        if(caminho.empty()){
            int obj[2] = {0,0};
            buscaUniforme(pose, obj);
            readCaminho(pose, obj);
            state = RETORNANDO;
        }
        else{
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
            return proxMovimento;
        }
    }

    if(state == RETORNANDO){
        //cout << "\testado -> " << state << "\tnum pacotes ->" << numPacotes <<endl;
        if(caminho.empty()){
            state = PREPARANDO_ROTA;
        }
        else{
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
            if(caminho.empty()){
                state = PREPARANDO_ROTA;
            }
            return proxMovimento;
        }
    }

    proxMovimento = RIGHT;
    return proxMovimento;
}

void socorrista::moveResult(int result, const int *newPose, float time, float bat) {
    pose[0] = newPose[0];
    pose[1] = newPose[1];
    tempoRestante = time;
    cargaBateriaAtual = bat;

    if(proxMovimento == SOLTAR_KIT){
        if(result == 1){
            victimsV.erase (victimsV.begin());
            if(caminho.empty()){
                int obj[2] = {0,0};
                buscaUniforme(pose, obj);
                readCaminho(pose, obj);
                state = RETORNANDO;
            }
        }
        else{
            int obj[2] = {0,0};
            while (!caminho.empty()){
                caminho.pop_front();
            }
            buscaUniforme(pose, obj);
            readCaminho(pose, obj);
            state = RETORNANDO;
        }
    }

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

void socorrista::setNumPacotesMax(int num) {
    numPacotesMax = num;
}

void socorrista::incNumPacotes() {
    numPacotes++;
    if(numPacotes > numPacotesMax)
        numPacotes = numPacotesMax;
}

int socorrista::decNumPacotes() {
    if(numPacotes <= 0){
        numPacotes = 0;
        return 0;
    }

    numPacotes--;
    return 1;
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
        for (int j = 0; j < 15; j++) {
            cout << victimsV[i][j] << " ";
        }
        cout << endl;
    }
}

void socorrista::printCaminho() {
    cout << "\t| ";
    for (std::list<point>::iterator it=caminho.begin(); it != caminho.end(); ++it){
        cout << it->x << ", " << it->y << " | ";
    }
    cout << endl;
}

void socorrista::printPopulation() {
    for (std::vector<int*>::iterator it=population.begin(); it != population.end(); ++it){
        cout << "chromossome \t-> ";
        for (int j=0; j<chromossomeSize; j++){
            cout << it[0][j] << " ";
        }
        cout << endl;
    }
}

int socorrista::buscaUniforme(const int *partida, const int *objetivo) {
    list<point> vizinhanca;
    point atual, aux;
    float custo;

    //cout << "partida: " << partida[0] << ", " << partida[1] << "\tobjetivo: " << objetivo[0] << ", " << objetivo[1] << endl;

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
    list<point> aux;
    point atual, prox;

    atual.x = objetivo[0];
    atual.y = objetivo[1];

    while ((atual.x != partida[0]) || (atual.y != partida[1])){
        aux.push_front(atual);
        prox.x = costs[atual.x][atual.y][0];
        prox.y = costs[atual.x][atual.y][1];

        atual.x = prox.x;
        atual.y = prox.y;
    }

    while (!aux.empty()){
        caminho.push_back(aux.front());
        aux.pop_front();
    }
}

vector<int*> socorrista::crossover(vector<int*> popSelec) {
    int prop = 0;
    vector<int*> popCross;
    int *chromossome1 = (int*) malloc(sizeof (int)*(victimsV.size()+1));
    int *chromossome2= (int*) malloc(sizeof (int)*(victimsV.size()+1));
    int *chromossomeAux = (int*) malloc(sizeof (int)*(victimsV.size()+1));
    int cross = 0;

    for (int i = 0; i<popSelec.size(); i=i+2){ // Para cada par de cromossomos
        int *chromossome1 = (int*) malloc(sizeof (int)*(victimsV.size()+1));
        int *chromossome2= (int*) malloc(sizeof (int)*(victimsV.size()+1));

        prop = rand() % 100 + 1; //Numero aleatorio de 1 a 100
        for (int j = 0; j < chromossomeSize; j++) {
            chromossome1[j] = popSelec[i][j];
            chromossome2[j] = popSelec[i+1][j];
        }

        if (prop <= probCrossover){
            //faz o crossover entre os dois
            cross = rand() % (victimsV.size() - 1); // numero aleatório entre 0 e victimsV.size() - 2
            for(int j=0;j<chromossomeSize; j++){
                if(j<=cross){
                    chromossomeAux[j] = chromossome1[j];
                    chromossome1[j] = chromossome2[j];
                    chromossome2[j] = chromossomeAux[j];
                }
            }
        }
       /* else{
            //adiciona os dois cromossomos sem crossover
            popCross.push_back(popSelec[i]);
            popCross.push_back(popSelec[i+1]);
        }*/
       fit(chromossome1);
       fit(chromossome2);
       popCross.push_back(chromossome1);
       popCross.push_back(chromossome2);
    }

    free(chromossomeAux);
    return popCross;
}

vector<int*> socorrista::mutation(vector<int *> popCross) {
    int prop = 0;
    vector<int*> popMutation;
    int *chromossome;

    //Para cada cromossomo
    for(int i=0;i<popCross.size(); i++){
        chromossome = popCross[i];

        //Para cada gene de um cromossomo, exceto o ultimo gene
        for(int j=0;j<(victimsV.size()); j++){
            prop = rand() % 100 + 1; //Numero aleatorio de 1 a 100
            if (prop <= probMutation){
                //faz a mutação do gene;
                if(chromossome[j] == 0){
                    chromossome[j] = 1;
                }
                else{
                    chromossome[j] = 0;
                }
            }
        }

        fit(chromossome);
        popMutation.push_back(chromossome);
    }

    return popMutation;
}

void socorrista::includePopulation(vector<int *> popMutation){
    int *chromossome;
    float tempoS;
    for(int i =0; i<popMutation.size();i++){
        chromossome=popMutation[i];
        tempoS=0;

        //cout << endl << "vitima " << i << endl << "\t";

        for(int j=0; j<(chromossomeSize-1); j++){
            //cout << chromossome[j] << " ";
            if(chromossome[j]==1){
                tempoS+=victimsV[j][14];
            }
        }
        if (tempoS <= tempoRestante){ //Se o tempo for menor do que o tempo que ele tem, inclui na população
            includeChromossome(chromossome);
        }
    }
}

void socorrista::includeChromossome(int *chromossome) {

    if(population.empty()){
        population.push_back(chromossome);
        return;
    }

    for (std::vector<int*>::iterator it=population.begin(); it != population.end(); ++it){
        if(it[0][chromossomeSize-1] < chromossome[chromossomeSize-1]){
            population.insert(it, chromossome);
            return;
        }
    }
    population.push_back(chromossome);
}

void socorrista::createFirstGen() {
    int *chromossome, flag, cont, gene, numGene;
    float saveTime;

    while(population.size() < numCrossover){
        chromossome = (int*) malloc(sizeof (int)*chromossomeSize);
        for (int i=0; i<chromossomeSize; i++){
            chromossome[i] = 0;
        }

        flag = victimsV.size();
        numGene = 0;
        while(flag > 0) {                       //tenta incluir uma vitima aleatória 10 vezes
            if(numGene == victimsV.size()){     //todas as vitimas foram incluidas
                break;
            }

            gene = rand() % (chromossomeSize - numGene - 1);
            flag--;
            //cout << "fl: " << flag << " gene: " << gene;

            for (cont = 0; cont < chromossomeSize; cont++) {
                if (chromossome[cont] == 1)
                    continue;
                if (gene == 0) {
                    chromossome[cont] = 1;
                    numGene++;
                    break;
                }
                gene--;
            }

            saveTime = 0;
            for(int i=0; i<chromossomeSize; i++){
                if(chromossome[i]==1){
                    saveTime+=victimsV[i][14];
                }
            }
            //cout << " tempo: " << saveTime;
            if (saveTime > tempoRestante){
                chromossome[cont] = 0;
                flag = 0;
                //cout << " ****";

                saveTime = 0;
                for(int i=0; i<chromossomeSize; i++){
                    if(chromossome[i]==1){
                        saveTime+=victimsV[i][14];
                    }
                }
                //cout << " tempo: " << saveTime;
            }

            //cout << endl;
        }
        fit(chromossome);
        includeChromossome(chromossome);
    }
}

int socorrista::fit(int *vet) {
    vet[chromossomeSize-1] = 0;
    for(int i=0; i<(chromossomeSize-1); i++){
        if(vet[i] == 1)
            vet[chromossomeSize-1]++;
    }
    return vet[chromossomeSize-1];
}

int* socorrista::getChromossomeSelec(){
    int* chromossome;
    chromossome = population[0];
    return chromossome;
}

int socorrista::getChromossomeSize(){
    return chromossomeSize;
}

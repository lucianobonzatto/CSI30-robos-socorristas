//
// Created by luciano on 12/03/2022.
//
#include "principal.h"

using namespace std;

principal::principal() {
    tempoVasculhador = 0;
    tempoSocorrista = 0;
    capacidadeSocorrista = 0;

    bateriaVasculhador = 0;
    bateriaSocorrista = 0;
    bateriaVasculhadorAtual = 0;
    bateriaSocorristaAtual = 0;
    initMap();
    initRobots();
    map.includeRobots(&roboV, &roboS);

    //map.printMap();
    //map.printVictims();
}

void principal::initRobots() {
    ifstream configFile ( "../config.txt", ios::in );
    string line;
    int mapSize[2] = {0,0};

    //read maxLin
    std::getline(configFile, line);
    for(int i=7; i<line.size(); i++){
        mapSize[0] *= 10;
        mapSize[0] += line[i] - '0';
    }

    //read maxCol
    std::getline(configFile, line);
    for(int i=7; i<line.size(); i++){
        mapSize[1] *= 10;
        mapSize[1] += line[i] - '0';
    }
    roboV.setMapSize(mapSize);
    roboS.setMapSize(mapSize);

    //read Tv
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        tempoVasculhador *= 10;
        tempoVasculhador += line[i] - '0';
    }
    roboV.setTime(tempoVasculhador);
    tempoVasculhadorTotal = tempoVasculhador;

    //read Ts
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        tempoSocorrista *= 10;
        tempoSocorrista += line[i] - '0';
    }
    roboS.setTime(tempoSocorrista);
    tempoSocorristaTotal = tempoSocorrista;

    //read Bv
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        bateriaVasculhador *= 10;
        bateriaVasculhador += line[i] - '0';
    }
    bateriaVasculhadorAtual = bateriaVasculhador;
    roboV.setBat(bateriaVasculhador);

    //read Bs
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        bateriaSocorrista *= 10;
        bateriaSocorrista += line[i] - '0';
    }
    bateriaSocorristaAtual= bateriaSocorrista;
    roboS.setBat(bateriaSocorrista);

    //read Ks
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        capacidadeSocorrista *= 10;
        capacidadeSocorrista += line[i] - '0';
    }
    roboS.setNumPacotesMax(capacidadeSocorrista);

    roboV.inicUntried();
    roboV.inicCoats();
    roboS.initMat();
}

void principal::initMap() {
    int mapSize[2], objPose[2];
    mapSize[0] = 0;
    mapSize[1] = 0;
    ifstream configFile ( "../config.txt", ios::in );
    ifstream ambienteFile ( "../ambiente.txt", ios::in );
    string line;

    //read maxLin
    std::getline(configFile, line);
    for(int i=7; i<line.size(); i++){
        mapSize[0] *= 10;
        mapSize[0] += line[i] - '0';
    }
    //read maxCol
    std::getline(configFile, line);
    for(int i=7; i<line.size(); i++){
        mapSize[1] *= 10;
        mapSize[1] += line[i] - '0';
    }

    //creat the map
    int** mat = (int**) malloc(mapSize[0]*sizeof(int*));
    for (int i = 0; i < mapSize[0]; i++){
        mat[i] = (int*) malloc(mapSize[1] * sizeof(int));
        for (int j = 0; j < mapSize[1]; j++) {
            mat[i][j] = 0;
        }
    }

    //read walls
    std::getline(ambienteFile, line);
    for(int i=6; i<line.size(); i++){
        if(line[i] == ' '){
            readCoord(line, i+1, objPose);
            if ((objPose[0] >= 0) && (objPose[1] >= 0) && (objPose[0] < mapSize[0]) && (objPose[1] < mapSize[1])) {
                mat[objPose[0]][objPose[1]] = -1;
                //cout << "parade -> " << objPose[0] << ',' << objPose[1] << endl;
            }
            else {
                cout << "posicao de parede invalida:" << objPose[0] << ',' << objPose[1] << endl;
            }
        }
    }

    //read victims
    int numVictims = 0;
    float* victim;
    while (std::getline(ambienteFile, line)){
        int i;
        i = readCoord(line, 7, objPose);
        numVictims++;
        victim = (float *) malloc(9*sizeof (float ));
        if ((objPose[0] >= 0) && (objPose[1] >= 0) && (objPose[0] < mapSize[0]) && (objPose[1] < mapSize[1])) {
            mat[objPose[0]][objPose[1]] = numVictims;
            victim[0] = (float) objPose[0];
            victim[1] = (float) objPose[1];
            //cout << "parade -> " << objPose[0] << ',' << objPose[1] << endl;
        }
        else {
            cout << "posicao de vitima invalida:" << objPose[0] << ',' << objPose[1] << endl;
            continue;
        }

        victim[2] = 0;
        int flg = 0;
        int j = 2;
        for(i++; i<line.size(); i++){
            if(line[i] == ' '){
                j++;
                victim[j] = 0;
                flg = 0;
            }
            else if(line[i] == '.'){
                flg = 10;
            }
            else {
                if(flg == 0){
                    victim[j] *= (float) 10;
                    victim[j] += (float) (line[i] - '0');
                }
                else {
                    victim[j] += ((float)line[i] - '0')/(float)flg;
                    flg *= 10;
                }
            }
        }
        map.includeVictim(victim);
    }

    map.setMap(mat, mapSize);
}

int principal::readCoord(string line, int firsVal, int *pose) {
    int eixo = 0, i;
    pose[0] = 0;
    pose[1] = 0;
    for(i=firsVal; i<line.size(); i++){
        if(line[i] == ','){
            eixo++;
        }
        else if(line[i] == ' ') {
            return i;
        }
        else{
            pose[eixo] *= 10;
            pose[eixo] += line[i] - '0';
        }
    }
    return i;
}

void principal::ciclo() {
    int move = DOWN, result=0;
    int nextPose[2];
    while(tempoVasculhador > 0){
        cout << "====================================================================================" << endl;
        cout<< "\ttempo " << tempoVasculhador << " | bateria " << bateriaVasculhadorAtual<< endl ;
        //cout << "tempo: " << tempoVasculhador << endl;
        //cout << "\t" << move << " -> " << result << endl;
        roboV.printMap();
        //map.printVictims();
        cout << endl;
        //map.printVictimasSalvas();

        move = roboV.moveDecision();
        cout << "move -> " << move << endl;
        result = tratMoveVasculhador(move, nextPose);
        if(bateriaVasculhadorAtual<=0 || tempoVasculhador <=0){
            break;
        }
        roboV.moveResult(result, nextPose, tempoVasculhador, bateriaVasculhadorAtual);
    }
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    int vascPose[2], socPose[2];
    roboV.getPose(vascPose);
    roboS.getPose(socPose);
    if(vascPose[0] == socPose[0] && vascPose[1] == socPose[1]){
        roboV.shareVictims(&roboS);
        roboV.shareMap(&roboS);
    }
    while(tempoSocorrista > 0){
        //cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        //cout<< "tempo " << tempoSocorrista << " | bateria " << bateriaSocorristaAtual<< endl;
        //roboS.printMap();
        //roboS.printCaminho();
        roboS.printMap();
        roboS.printVictims();
        cout << endl;
        map.printVictimasSalvas();

        move = roboS.moveDecision();
        if(move == FINALIZAR_BUSCA){
            break;
        }

        result = tratMoveSocorrista(move, nextPose);
        if(bateriaSocorristaAtual<=0 || tempoSocorrista <=0){
            break;
        }
        roboS.moveResult(result, nextPose, tempoSocorrista, bateriaSocorristaAtual);
    }

    tempoVasculhadorTotal -= tempoVasculhador;
    tempoSocorristaTotal -= tempoSocorrista;
    cout << "Numero de vitimas localizadas por tempo gasto" << endl;
    cout << "\t" << roboV.getNumVitimas()/tempoVasculhadorTotal << endl;

    cout << "Numero de vitimas salvas por tempo gasto" << endl;
    cout << "\t As -> " << map.getNumVitimasSalvas()/tempoSocorristaTotal << endl;
    cout << "\t dupla -> " << map.getNumVitimasSalvas()/(tempoVasculhadorTotal+tempoSocorristaTotal) << endl;

    cout << "Numero de vitimas localizadas em 10 extratos de gravidade" << endl;
    roboV.printVictimsGrav();

    cout  << endl << "Numero de vitimas salvas em 10 extratos de gravidade" << endl;
    map.printVictimasSalvasGrav();
}

int principal::tratMoveVasculhador(int move, int nextPose[2]) {
    int poseVasculhador[2];
    float* victim;
    roboV.getPose(poseVasculhador);
    roboV.getPose(nextPose);
    switch (move) {
        case RECARREGAR:
            tempoVasculhador = tempoVasculhador - 240;
            bateriaVasculhadorAtual = bateriaVasculhador;
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1];
            return -3;
        case READ_VICTIM:
            tempoVasculhador -= 2;
            bateriaVasculhadorAtual -= 2;
            victim = map.getVictim(poseVasculhador[0],poseVasculhador[1]);
            roboV.includeVictim(victim);
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1];
            return 1;
        case DOWN:
            tempoVasculhador--;
            bateriaVasculhadorAtual--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1];
            return map.tryMoveVasculhador(move);
        case UP:
            tempoVasculhador--;
            bateriaVasculhadorAtual--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1];
            return map.tryMoveVasculhador(move);
        case RIGHT:
            tempoVasculhador--;
            bateriaVasculhadorAtual--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] + 1;
            return map.tryMoveVasculhador(move);
        case LEFT:
            tempoVasculhador--;
            bateriaVasculhadorAtual--;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0];
            nextPose[1] = poseVasculhador[1] - 1;
            return map.tryMoveVasculhador(move);
        case UP_RIGHT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhadorAtual = bateriaVasculhadorAtual - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] + 1;
            return map.tryMoveVasculhador(move);
        case UP_LEFT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhadorAtual = bateriaVasculhadorAtual - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] - 1;
            nextPose[1] = poseVasculhador[1] - 1;
            return map.tryMoveVasculhador(move);
        case DOWN_RIGHT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhadorAtual = bateriaVasculhadorAtual - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] + 1;
            return map.tryMoveVasculhador(move);
        case DOWN_LEFT:
            tempoVasculhador = tempoVasculhador - 1.5;
            bateriaVasculhadorAtual = bateriaVasculhadorAtual - 1.5;
            if (map.tryMoveVasculhador(move) == -1)
                return -1;
            nextPose[0] = poseVasculhador[0] + 1;
            nextPose[1] = poseVasculhador[1] - 1;
            return map.tryMoveVasculhador(move);
        default:
            tempoVasculhador--;
            bateriaVasculhadorAtual--;
            return -1;
    }
}

int principal::tratMoveSocorrista(int move, int *nextPose) {
    int poseSocorrista[2];
    float* victim;
    roboS.getPose(poseSocorrista);
    roboS.getPose(nextPose);
    switch (move) {
        case SOLTAR_KIT:
            tempoSocorrista -= 0.5;
            bateriaSocorristaAtual -= 0.5;
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1];
            if(roboS.decNumPacotes()){
                map.socorristaSoltouKit();
                return 1;
            }
            return 0;
        case CARREGAR_KIT:
            tempoSocorrista -= 0.5;
            bateriaSocorristaAtual -= 0.5;
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1];
            roboS.incNumPacotes();
            return -3;
        case RECARREGAR:
            tempoSocorrista -= 240;
            bateriaSocorristaAtual = bateriaSocorrista;
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1];
            return -3;
        case DOWN:
            tempoSocorrista--;
            bateriaSocorristaAtual--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1];
            return map.tryMoveSocorrista(move);
        case UP:
            tempoSocorrista--;
            bateriaSocorristaAtual--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1];
            return map.tryMoveSocorrista(move);
        case RIGHT:
            tempoSocorrista--;
            bateriaSocorristaAtual--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1] + 1;
            return map.tryMoveSocorrista(move);
        case LEFT:
            tempoSocorrista--;
            bateriaSocorristaAtual--;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0];
            nextPose[1] = poseSocorrista[1] - 1;
            return map.tryMoveSocorrista(move);
        case UP_RIGHT:
            tempoSocorrista -= 1.5;
            bateriaSocorristaAtual -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1] + 1;
            return map.tryMoveSocorrista(move);
        case UP_LEFT:
            tempoSocorrista -= 1.5;
            bateriaSocorristaAtual -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] - 1;
            nextPose[1] = poseSocorrista[1] - 1;
            return map.tryMoveSocorrista(move);
        case DOWN_RIGHT:
            tempoSocorrista -= 1.5;
            bateriaSocorristaAtual -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1] + 1;
            return map.tryMoveSocorrista(move);
        case DOWN_LEFT:
            tempoSocorrista -= 1.5;
            bateriaSocorristaAtual -= 1.5;
            if (map.tryMoveSocorrista(move) == -1)
                return -1;
            nextPose[0] = poseSocorrista[0] + 1;
            nextPose[1] = poseSocorrista[1] - 1;
            return map.tryMoveSocorrista(move);
        default:
            tempoSocorrista--;
            bateriaSocorristaAtual--;
            return -1;
    }
}
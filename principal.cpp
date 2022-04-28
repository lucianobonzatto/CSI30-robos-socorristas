//
// autores: Luciano Bonzatto Junior e Juliana Martins Ferreira
// professor: Cesar Augusto Tacla
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
    /*std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        bateriaVasculhador *= 10;
        bateriaVasculhador += line[i] - '0';
    }*/
    bateriaVasculhador = tempoVasculhador;
    bateriaVasculhadorAtual = bateriaVasculhador;
    roboV.setBat(bateriaVasculhador);

    //read Bs
    /*std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        bateriaSocorrista *= 10;
        bateriaSocorrista += line[i] - '0';
    }*/
    bateriaSocorrista = tempoSocorrista;
    bateriaSocorristaAtual= bateriaSocorrista;
    roboS.setBat(bateriaSocorrista);

    //read Ks
    std::getline(configFile, line);
    for(int i=3; i<line.size(); i++){
        capacidadeSocorrista *= 10;
        capacidadeSocorrista += line[i] - '0';
    }
    roboS.setNumPacotesMax(capacidadeSocorrista);

    configFile.close();

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
    ifstream difAcessoFile ( "../difacesso.txt", ios::in );
    ifstream sinaisVitaisFile ( "../sinaisvitais.txt", ios::in );
    string line, sinaisVitais, difAcesso;

    std::getline(ambienteFile, line); // ignora a primeira linha

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


    //read victims
    int numVictims = 0, cont = 6;
    float* victim;
    std::getline(ambienteFile, line);
    while (cont < line.size()){
        cont++;
        cont = readCoord(line, cont, objPose);

        numVictims++;
        victim = (float *) malloc(15*sizeof (float ));
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
        std::getline(sinaisVitaisFile, sinaisVitais);
        for(int i=0; i<sinaisVitais.size(); i++){
            if(sinaisVitais[i] == ' '){
                j++;
                victim[j] = 0;
                flg = 0;
            }
            else if(sinaisVitais[i] == '.'){
                flg = 10;
            }
            else {
                if(flg == 0){
                    victim[j] *= (float) 10;
                    victim[j] += (float) (sinaisVitais[i] - '0');
                }
                else {
                    victim[j] += ((float)sinaisVitais[i] - '0')/(float)flg;
                    flg *= 10;
                }
            }
        }

        j=8;
        std::getline(difAcessoFile, difAcesso);
        for(int i=0; i<difAcesso.size(); i++){
            if(difAcesso[i] == ' '){
                j++;
                victim[j] = 0;
                flg = 0;
            }
            else if(difAcesso[i] == '.'){
                flg = 10;
            }
            else {
                if(flg == 0){
                    victim[j] *= (float) 10;
                    victim[j] += (float) (difAcesso[i] - '0');
                }
                else {
                    victim[j] += ((float)difAcesso[i] - '0')/(float)flg;
                    flg *= 10;
                }
            }
        }
        map.includeVictim(victim);
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

    map.setMap(mat, mapSize);
    configFile.close();
    ambienteFile.close();
    difAcessoFile.close();
    sinaisVitaisFile.close();
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
    /*while(tempoVasculhador > 0){
        //cout << "====================================================================================" << endl;
        //cout<< "\ttempo " << tempoVasculhador << " | bateria " << bateriaVasculhadorAtual<< endl ;
        //cout << "tempo: " << tempoVasculhador << endl;
        //cout << "\t" << move << " -> " << result << endl;
        //roboV.printMap();
        //roboV.printVictims();
        //cout << endl;
        //map.printVictimasSalvas();

        move = roboV.moveDecision();
        //cout << "move -> " << move << endl;
        result = tratMoveVasculhador(move, nextPose);
        if(bateriaVasculhadorAtual<=0 || tempoVasculhador <=0){
            break;
        }
        roboV.moveResult(result, nextPose, tempoVasculhador, bateriaVasculhadorAtual);
    }

    cout << "====================================================================================" << endl;
    roboV.printMap();
    roboV.printVictims();

    int vascPose[2], socPose[2];
    roboV.getPose(vascPose);
    roboS.getPose(socPose);
    if(vascPose[0] == socPose[0] && vascPose[1] == socPose[1]){
        roboV.shareVictims(&roboS);
        roboV.shareMap(&roboS);
    }*/

    map.shareVictims(&roboS);
    map.shareMap(&roboS);

    while(tempoSocorrista > 0){
        //cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        //cout<< "tempo " << tempoSocorrista << " | bateria " << bateriaSocorristaAtual<< endl;
        //roboS.printMap();
        //roboS.printCaminho();
        //roboS.printMap();
        //roboS.printVictims();
        //cout << endl;
        //map.printVictimasSalvas();

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
    //cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    //roboS.printMap();
    //roboS.printVictims();
    //roboS.printPopulation();

    int* crm = roboS.getChromossomeSelec();
    /*for(int i = 0; i<roboS.getChromossomeSize(); i++){
        cout << "[" << crm[i] << "] ";
    }*/

    //cout << endl << "------------------------------------------------------------------------------------" << endl;

    float ts = 0;
    float G = 0;
    int numVitimas = crm[roboS.getChromossomeSize()-1];
    float Se=0;
    int s5=0, s4=0, s3=0, s2=0, s1=0;
    int S5=0, S4=0, S3=0, S2=0, S1=0;

    vector<float*> victims = map.getVictims();

    for(int j=0; j<(roboS.getChromossomeSize()-1); j++){
        if(crm[j]==1){
            ts+=victims[j][14];
            G+=victims[j][7];
            if(victims[j][7] >=0 && victims[j][7] < 0.2)
                s1++;
            if(victims[j][7] >=0.2 && victims[j][7] < 0.4)
                s2++;
            if(victims[j][7] >=0.4 && victims[j][7] < 0.6)
                s3++;
            if(victims[j][7] >=0.6 && victims[j][7] < 0.8)
                s4++;
            if(victims[j][7] >=0.8 && victims[j][7] < 1)
                s5++;
        }

        if(victims[j][7] >=0 && victims[j][7] < 0.2)
            S1++;
        if(victims[j][7] >=0.2 && victims[j][7] < 0.4)
            S2++;
        if(victims[j][7] >=0.4 && victims[j][7] < 0.6)
            S3++;
        if(victims[j][7] >=0.6 && victims[j][7] < 0.8)
            S4++;
        if(victims[j][7] >=0.8 && victims[j][7] < 1)
            S5++;
    }

    Se= (5*s5 + 4*s4 + 3*s3 + 2*s2 + s1) / (ts*(5*S5 + 4*S4 + 3*S3 + 2*S2 + S1));
    //cout << numVitimas << "\n" << ts << "\n" << map.getNumVitimas() << "\n" << G << "\n" << G/ts << "\n" << Se << endl;

    cout << "numero de vitimas salvas: " << numVitimas << endl;
    cout << "tempo gasto pelo socorrista: " << ts << endl;
    cout << "numero total de vitimas: " << map.getNumVitimas() << endl;
    cout << "gravidade acumulada das vitimas salvas: " << G << endl;
    cout << "numero de vitimas salvas pelo tempo gasto: " << G/ts << endl;
    cout << "Numero de vitimas salvas em 5 extratos de gravidade pelo tempo gasto: " << Se << endl;

    cout << "\t[0.0] [0.2] - " << s1 << endl;
    cout << "\t[0.2] [0.4] - " << s2 << endl;
    cout << "\t[0.4] [0.6] - " << s3 << endl;
    cout << "\t[0.6] [0.8] - " << s4 << endl;
    cout << "\t[0.8] [1] - " << s5 << endl;


    /*tempoVasculhadorTotal -= tempoVasculhador;
    tempoSocorristaTotal -= tempoSocorrista;
    cout << "Numero de vitimas localizadas por tempo gasto" << endl;
    cout << "\t" << roboV.getNumVitimas()/tempoVasculhadorTotal << endl;

    cout << "Numero de vitimas salvas por tempo gasto" << endl;
    cout << "\t As -> " << map.getNumVitimasSalvas()/tempoSocorristaTotal << endl;
    cout << "\t dupla -> " << map.getNumVitimasSalvas()/(tempoVasculhadorTotal+tempoSocorristaTotal) << endl;

    cout << "Numero de vitimas localizadas em 10 extratos de gravidade" << endl;
    roboV.printVictimsGrav();

    cout  << endl << "Numero de vitimas salvas em 10 extratos de gravidade" << endl;
    map.printVictimasSalvasGrav();*/
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
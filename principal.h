//
// Created by lucia on 12/03/2022.
//

#ifndef TAREFA_1_PRINCIPAL_H
#include <fstream>
#include <string>

#include "ambiente.h"
#include "explorador.h"
#define TAREFA_1_PRINCIPAL_H

class principal {
private:
    ambiente map;
    explorador roboV;
    int readCoord(string line, int firsVal, int pose[2]);

public:
    principal();
};

#endif //TAREFA_1_PRINCIPAL_H

//
// Created by nikolas on 8/6/22.
//

#ifndef COMP371_PROJECT_COLOURROOM_NOMAIN_H
#define COMP371_PROJECT_COLOURROOM_NOMAIN_H

#define _USE_MATH_DEFINES //enables us to use pi


#include <iostream>
#include <math.h>
#include <random>

#include "../../Config.h"

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
// initializing OpenGL and binding inputs

#include "../include/shaderloader.h"

namespace colorRoom
{

    void createWindow();
    void colourroom();
}
#endif //COMP371_PROJECT_COLOURROOM_NOMAIN_// H

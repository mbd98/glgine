//
// Created by nikolas on 8/6/22.
//

#ifndef COMP371_PROJECT_COLOURROOM_NOMAIN_H
#define COMP371_PROJECT_COLOURROOM_NOMAIN_H

#define _USE_MATH_DEFINES //enables us to use pi


#include <iostream>
#include <math.h>
#include <random>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include "../include/shaderloader.h"

namespace colorRoom
{
    void createWindow();
    void colourroom();
}
#endif

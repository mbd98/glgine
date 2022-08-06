//
// Created by nikolas on 2/24/22.
//

#ifndef MINECRAFTOPENGL_SHADER_H
#define MINECRAFTOPENGL_SHADER_H

#include "Config.h"
class Shader {

public:
    static void readShader(const std::string &filepath, std::stringstream &ss);
    static GLuint compileShader(const std::string &source, GLuint type);
    static GLuint CreateShader(const std::string &vert, const std::string &frag);
};


#endif //MINECRAFTOPENGL_SHADER_H

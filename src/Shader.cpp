//
// Created by nikolas on 2/24/22.
//

#include "Shader.h"

void Shader::readShader(const std::string &filepath, std::stringstream &ss) {
    std::ifstream inputFile(filepath);
    std::string line;

    while (getline(inputFile, line)) {
        ss << line << "\n";
    }
}

GLuint Shader::compileShader(const std::string &source, GLuint type) {
    GLuint id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);

    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char *msg = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, msg);

        std::cerr << "failed to compile " << msg << "\n";
        glDeleteShader(id);
        return 0;
    }
    return id;
}

GLuint Shader::CreateShader(const std::string &vert_path, const std::string &frag_path) {
    GLuint program = glCreateProgram();

    std::stringstream vert,frag;
    readShader(vert_path,vert);
    readShader(frag_path,frag);

    GLuint vert_shader = compileShader(vert.str(), GL_VERTEX_SHADER);
    GLuint frag_shader = compileShader(frag.str(), GL_FRAGMENT_SHADER);

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;


}


//
// Created by nikolas on 2/23/22.
//

#ifndef MINECRAFTOPENGL_CONFIG_H
#define MINECRAFTOPENGL_CONFIG_H

#include <glm/gtc/matrix_transform.hpp>

#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include "sstream"
#include <iostream>
#include "GL/glew.h"
#include "GL/gl.h"
#include "GLFW/glfw3.h"

#define ASSERT(x) if (!(x)) raise(SIGTRAP);

constexpr float VIEWPORT_HEIGHT = 1700;
constexpr float ASPECT_RATIO = 1.78;
constexpr int VIEWPORT_WIDTH = VIEWPORT_HEIGHT * ASPECT_RATIO;


// Logging stuff. Wrapping opengl with these functions will let the user know if there were any error.s
static bool GLLogCall(const char* function,int line){
    if(GLenum error = glGetError()){
        std::cout << "[Open GL error] (" << error << ") on line : " << line << " from function " << function << std::endl;;
        return false;
    }
    return true;
}
static void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}
#define GLCheck(x) GLClearError(); \
    x;                             \
    ASSERT(GLLogCall(#x,__LINE__))

#define PI 3.14159265358f

constexpr unsigned int SHADOW_WIDTH = 1024;
constexpr unsigned int SHADOW_HEIGHT = 1024;

constexpr float DEFAULT_WHEEL_ANGLE = 0.f;
constexpr glm::vec3 DEFAULT_POSITION = {0.f,0.5f,0.f};
constexpr glm::vec2 DEFAULT_WORLD_ORIENTATION = {0.f,0.f};
constexpr glm::vec3 DEFAULT_EYE_POSITION = {0.f,3.f,4.f};
constexpr glm::vec2 DEFAULT_WORLD_TRANSLATION = {0.f,0.f};
constexpr float DEFAULT_MODEL_ORIENTATION = 0.0f;
constexpr glm::vec3 DEFAULT_MODEL_POSITION = {0.0,0.0,0.0};
constexpr GLfloat DEFAULT_OLAF_ORIENTATION = 0.0f;
constexpr GLfloat DEFAULT_SCALE = 1.0f;
constexpr glm::vec3 DEFAULT_LIGHT_POSITION = {0.001,2,3};
constexpr float DEFAULT_ZOOM = 75.0f;
enum Mode{
    TRIANGLES = GL_TRIANGLES,
    LINES = GL_LINES,
    POINTS = GL_POINTS
};

static glm::vec3 random_color(){
    return glm::vec3{ (double)rand() / (double)RAND_MAX,(double)rand() / (double)RAND_MAX, (double)rand() / (double)RAND_MAX };
}
#define CUBE_DRAW_SIZE 108

#endif //MINECRAFTOPENGL_CONFIG_H
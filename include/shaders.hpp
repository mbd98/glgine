#pragma once

#include "defaults.hpp"

extern GLuint loadShader(const char*, const char*);
extern void setUniformMat4(GLuint, const char*, const glm::mat4&);
extern void setUniformVec2(GLuint, const char*, const glm::vec2&);
extern void setUniformVec3(GLuint, const char*, const glm::vec3&);
extern void setUniformVec4(GLuint, const char*, const glm::vec4&);
extern void setUniformFloat(GLuint, const char*, GLfloat);
extern void setUniformInt(GLuint, const char*, GLint);

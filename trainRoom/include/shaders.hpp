#pragma once

#include "defaults.hpp"

extern const char * const MODEL;
extern const char * const VIEW;
extern const char * const PROJECTION;
extern const char * const LIGHT0;
extern const char * const LIGHT1;
extern const char * const LIGHT2;
extern const char * const TEXTURE_DIFFUSE0;
extern const char * const TEXTURE_SPECULAR0;
extern const char * const SHADOW_MAP;
//extern const char * const LIGHT_COLOR;
extern const char * const LIGHT0_POSITION;
extern const char * const LIGHT0_DIRECTION;
extern const char * const LIGHT0_AMBIENT;
extern const char * const LIGHT0_DIFFUSE;
extern const char * const LIGHT0_SPECULAR;
extern const char * const LIGHT1_POSITION;
extern const char * const LIGHT1_DIRECTION;
extern const char * const LIGHT1_AMBIENT;
extern const char * const LIGHT1_DIFFUSE;
extern const char * const LIGHT1_SPECULAR;
extern const char * const LIGHT2_POSITION;
extern const char * const LIGHT2_DIRECTION;
extern const char * const LIGHT2_AMBIENT;
extern const char * const LIGHT2_DIFFUSE;
extern const char * const LIGHT2_SPECULAR;
extern const char * const AMBIENT_STRENGTH;
extern const char * const DIFFUSE_STRENGTH;
extern const char * const SPECULAR_STRENGTH;
extern const char * const VIEW_POSITION;
extern const char * const DO_LIGHTING;
extern const char * const LIGHT_INNER;
extern const char * const LIGHT_OUTER;
extern const char * const SHINE;

extern GLuint loadShader(const char*, const char*);
extern void setUniformMat4(GLuint, const char*, const glm::mat4&);
extern void setUniformVec2(GLuint, const char*, const glm::vec2&);
extern void setUniformVec3(GLuint, const char*, const glm::vec3&);
extern void setUniformVec4(GLuint, const char*, const glm::vec4&);
extern void setUniformFloat(GLuint, const char*, GLfloat);
extern void setUniformInt(GLuint, const char*, GLint);

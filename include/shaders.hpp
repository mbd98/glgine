#pragma once

#include "defaults.hpp"

extern const char * const MODEL;
extern const char * const VIEW;
extern const char * const PROJECTION;
extern const char * const LIGHT;
extern const char * const TEXTURE_MAP;
extern const char * const SHADOW_MAP;
extern const char * const LIGHT_COLOR;
extern const char * const LIGHT_POSITION;
extern const char * const LIGHT_DIRECTION;
extern const char * const AMBIENT_STRENGTH;
extern const char * const DIFFUSE_STRENGTH;
extern const char * const SPECULAR_STRENGTH;
extern const char * const VIEW_POSITION;
extern const char * const TEXTURE_PRESENT;
extern const char * const OBJECT_COLOR;
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

//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

//Resources consulted:
//My code for Assignment 1 and Quiz 1, which was based on the code structure provided in the lab, and in turn, used the following as references:
//Code provided during the lab
//GLFW documentation (https://www.glfw.org/documentation), specifically sections regarding user input and keys
//glm documentation (https://glm.g-truc.net/0.9.9/api/index.html)
//The Learn OpenGL tutorial (linked above).

//Ryan Rokni (40133051)

#define _USE_MATH_DEFINES //enables us to use pi


#include <iostream>
#include <math.h>
#include <random>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

#include "../include/shaderloader.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"


float unit = 0.1;
float unit2 = 0.7;

struct TexturedColoredVertex
{
    TexturedColoredVertex(glm::vec3 _position, glm::vec3 _color, glm::vec2 _uv, glm::vec3 _normal)
    : position(_position), color(_color), uv(_uv), normal(_normal) {}

    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 uv;
	glm::vec3 normal;
};

//Holds the coordinate points/colour for one grid line, which then gets instantiated to produce the rest.
//The y-coordinate is slightly under zero so that other elements (such as the axis) appear above it.
glm::vec3 gridlineArray[] = {
	glm::vec3(unit*50.0f, unit*-0.001f, unit*50.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(unit*50.0f, unit*-0.001f, -unit*50.0f),
	glm::vec3(1.0f, 1.0f, 0.0f)
};

//Holds the coordinates/colours for each of the axis lines
glm::vec3 axisArray[] = {
	//X-axis, Red
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(unit*5.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),

	//Y-axis, Green
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, unit*5.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	
	//Z-axis, Blue
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, unit*5.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
};

//Holds the coordinates/colours for triangles that form a sphere. Adapted from Lab09 code.
TexturedColoredVertex unitSphereArray[] = {
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.173648, -0.000000, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, -0.000000, -0.984808)),
TexturedColoredVertex(glm::vec3(0.171010, -0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(0.342020, -0.000000, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, -0.000000, -0.939693)),
TexturedColoredVertex(glm::vec3(0.336824, -0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(0.500000, -0.000000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, -0.000000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.492404, -0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(0.642788, -0.000000, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, -0.000000, -0.766044)),
TexturedColoredVertex(glm::vec3(0.633022, -0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(0.766044, -0.000000, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, -0.000000, -0.642788)),
TexturedColoredVertex(glm::vec3(0.754407, -0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.866025, -0.000000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, -0.000000, -0.500000)),
TexturedColoredVertex(glm::vec3(0.852869, -0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(0.939693, -0.000000, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, -0.000000, -0.342020)),
TexturedColoredVertex(glm::vec3(0.925417, -0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(0.984808, -0.000000, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, -0.000000, -0.173648)),
TexturedColoredVertex(glm::vec3(0.969846, -0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, -0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(1.000000, -0.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(1.000000, -0.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.984808, -0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, -0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(0.984808, -0.000000, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, -0.000000, 0.173648)),
TexturedColoredVertex(glm::vec3(0.969846, -0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, -0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(0.939693, -0.000000, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, -0.000000, 0.342020)),
TexturedColoredVertex(glm::vec3(0.925417, -0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(0.866025, -0.000000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, -0.000000, 0.500000)),
TexturedColoredVertex(glm::vec3(0.852869, -0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(0.766044, -0.000000, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, -0.000000, 0.642788)),
TexturedColoredVertex(glm::vec3(0.754407, -0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.642788, -0.000000, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, -0.000000, 0.766044)),
TexturedColoredVertex(glm::vec3(0.633022, -0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(0.500000, -0.000000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, -0.000000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.492404, -0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(0.342020, -0.000000, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, -0.000000, 0.939693)),
TexturedColoredVertex(glm::vec3(0.336824, -0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(0.173648, -0.000000, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, -0.000000, 0.984808)),
TexturedColoredVertex(glm::vec3(0.171010, -0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.171010, -0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(0.163176, -0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(0.336824, -0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(0.321394, -0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(0.492404, -0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(0.469846, -0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(0.633022, -0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(0.604023, -0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(0.754407, -0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.719846, -0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(0.852869, -0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(0.813798, -0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(0.925417, -0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(0.883022, -0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, -0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(0.969846, -0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, -0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(0.925417, -0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(0.984808, -0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, -0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(0.939693, -0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, -0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(0.969846, -0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, -0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(0.925417, -0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(0.925417, -0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(0.883022, -0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, -0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(0.852869, -0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(0.813798, -0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(0.754407, -0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.719846, -0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(0.633022, -0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(0.604023, -0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(0.492404, -0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(0.469846, -0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(0.336824, -0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(0.321394, -0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, -0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(0.163176, -0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.163176, -0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(0.150384, -0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(0.321394, -0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(0.296198, -0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(0.469846, -0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(0.433013, -0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.604023, -0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(0.556670, -0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(0.719846, -0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(0.663414, -0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.813798, -0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(0.750000, -0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, -0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(0.883022, -0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, -0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(0.813798, -0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.925417, -0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(0.852869, -0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(0.939693, -0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, -0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(0.866025, -0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, -0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.925417, -0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, -0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(0.852869, -0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(0.883022, -0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, -0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(0.813798, -0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.813798, -0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(0.750000, -0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, -0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(0.719846, -0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(0.663414, -0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.604023, -0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(0.556670, -0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(0.469846, -0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(0.433013, -0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, -0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(0.296198, -0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(0.163176, -0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(0.150384, -0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.150384, -0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(0.133022, -0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(0.296198, -0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(0.262003, -0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(0.433013, -0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.383022, -0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(0.556670, -0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(0.492404, -0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(0.663414, -0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.586824, -0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, -0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(0.750000, -0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, -0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(0.663414, -0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(0.813798, -0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.719846, -0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(0.852869, -0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(0.754407, -0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(0.866025, -0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, -0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.766044, -0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, -0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(0.852869, -0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, -0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(0.754407, -0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(0.813798, -0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, -0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.719846, -0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(0.750000, -0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, -0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(0.663414, -0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(0.663414, -0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.586824, -0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, -0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(0.556670, -0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(0.492404, -0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(0.433013, -0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.383022, -0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(0.296198, -0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(0.262003, -0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(0.150384, -0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(0.133022, -0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.133022, -0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(0.111619, -0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(0.262003, -0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(0.219846, -0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(0.383022, -0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, -0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(0.492404, -0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(0.413176, -0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, -0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(0.586824, -0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, -0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(0.492404, -0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(0.663414, -0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(0.556670, -0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(0.719846, -0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(0.604023, -0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.754407, -0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(0.633022, -0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(0.766044, -0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, -0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(0.642788, -0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, -0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(0.754407, -0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, -0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(0.633022, -0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(0.719846, -0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, -0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(0.604023, -0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.663414, -0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, -0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(0.556670, -0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(0.586824, -0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, -0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(0.492404, -0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(0.492404, -0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(0.413176, -0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, -0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(0.383022, -0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, -0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(0.262003, -0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(0.219846, -0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(0.133022, -0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(0.111619, -0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.111619, -0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(0.086824, -0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(0.219846, -0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, -0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(0.321394, -0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(0.250000, -0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, -0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(0.413176, -0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, -0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(0.321394, -0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(0.492404, -0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(0.383022, -0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(0.556670, -0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(0.433013, -0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(0.604023, -0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.469846, -0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(0.633022, -0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(0.492404, -0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(0.642788, -0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, -0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(0.500000, -0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, -0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(0.633022, -0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, -0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(0.492404, -0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(0.604023, -0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, -0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.469846, -0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(0.556670, -0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, -0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(0.433013, -0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(0.492404, -0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(0.383022, -0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(0.413176, -0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, -0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(0.321394, -0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(0.321394, -0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(0.250000, -0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, -0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(0.219846, -0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, -0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(0.111619, -0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(0.086824, -0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.086824, -0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(0.059391, -0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(0.171010, -0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(0.116978, -0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, -0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(0.250000, -0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, -0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(0.171010, -0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, -0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(0.219846, -0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(0.383022, -0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(0.262003, -0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(0.433013, -0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(0.296198, -0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(0.469846, -0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(0.321394, -0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(0.492404, -0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(0.336824, -0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(0.500000, -0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, -0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(0.342020, -0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, -0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(0.492404, -0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, -0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(0.336824, -0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(0.469846, -0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, -0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(0.321394, -0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(0.433013, -0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, -0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(0.296198, -0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(0.383022, -0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, -0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(0.262003, -0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(0.321394, -0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(0.219846, -0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(0.250000, -0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, -0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(0.171010, -0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(0.171010, -0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(0.116978, -0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, -0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(0.086824, -0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(0.059391, -0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.059391, -0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(0.030154, -0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, -0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(0.116978, -0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, -0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(0.059391, -0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, -0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(0.086824, -0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(0.219846, -0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(0.111619, -0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(0.262003, -0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(0.133022, -0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(0.296198, -0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(0.150384, -0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(0.321394, -0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(0.163176, -0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(0.336824, -0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(0.171010, -0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(0.342020, -0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, -0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(0.173648, -0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, -0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(0.336824, -0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, -0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(0.171010, -0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(0.321394, -0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, -0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(0.163176, -0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(0.296198, -0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, -0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(0.150384, -0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(0.262003, -0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, -0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(0.133022, -0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(0.219846, -0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, -0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(0.111619, -0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(0.171010, -0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(0.086824, -0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(0.116978, -0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, -0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(0.059391, -0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(0.059391, -0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(0.030154, -0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, -0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.030154, -0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, -0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.173648, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.173648, -0.984808)),
TexturedColoredVertex(glm::vec3(0.059391, -0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.342020, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.342020, -0.939693)),
TexturedColoredVertex(glm::vec3(0.086824, -0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.500000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.500000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.111619, -0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.642788, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.642788, -0.766044)),
TexturedColoredVertex(glm::vec3(0.133022, -0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.766044, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.766044, -0.642788)),
TexturedColoredVertex(glm::vec3(0.150384, -0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.866025, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.866025, -0.500000)),
TexturedColoredVertex(glm::vec3(0.163176, -0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.939693, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.939693, -0.342020)),
TexturedColoredVertex(glm::vec3(0.171010, -0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.984808, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.984808, -0.173648)),
TexturedColoredVertex(glm::vec3(0.173648, -0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, -0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.000000, -1.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -1.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.171010, -0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, -0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.984808, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.984808, 0.173648)),
TexturedColoredVertex(glm::vec3(0.163176, -0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, -0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.939693, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.939693, 0.342020)),
TexturedColoredVertex(glm::vec3(0.150384, -0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, -0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.866025, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.866025, 0.500000)),
TexturedColoredVertex(glm::vec3(0.133022, -0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, -0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.766044, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.766044, 0.642788)),
TexturedColoredVertex(glm::vec3(0.111619, -0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, -0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.642788, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.642788, 0.766044)),
TexturedColoredVertex(glm::vec3(0.086824, -0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, -0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.500000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.500000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.059391, -0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, -0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.342020, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.342020, 0.939693)),
TexturedColoredVertex(glm::vec3(0.030154, -0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, -0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.173648, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.173648, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.173648, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.173648, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.030154, -0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, -0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.342020, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.342020, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.500000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.500000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.642788, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.642788, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.766044, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.766044, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.866025, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.866025, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.939693, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.939693, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.984808, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.984808, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.000000, -1.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -1.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.173648, -0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, -0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.984808, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.984808, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.939693, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.939693, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.866025, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.866025, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.766044, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.766044, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.642788, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.642788, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.500000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.500000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.342020, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.342020, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.000000, -0.173648, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.000000, -0.173648, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.030154, -0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, -0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.030154, -0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, -0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.116978, -0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, -0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.173648, -0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, -0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.342020, -0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, -0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.116978, -0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, -0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.030154, -0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, -0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.116978, -0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, -0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.250000, -0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, -0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.342020, -0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, -0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.500000, -0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, -0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.250000, -0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, -0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.116978, -0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, -0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.059391, -0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, -0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.250000, -0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, -0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.413176, -0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, -0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.500000, -0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, -0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.642788, -0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, -0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.413176, -0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, -0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.250000, -0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, -0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.086824, -0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, -0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.413176, -0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, -0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.586824, -0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, -0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.642788, -0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, -0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.766044, -0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, -0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.586824, -0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, -0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.413176, -0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, -0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.219846, -0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, -0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.111619, -0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, -0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.586824, -0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, -0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.750000, -0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, -0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.766044, -0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, -0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.866025, -0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, -0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.750000, -0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, -0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.586824, -0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, -0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.383022, -0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, -0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.262003, -0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, -0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.133022, -0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, -0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.750000, -0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, -0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.883022, -0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, -0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.866025, -0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, -0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.939693, -0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, -0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.883022, -0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, -0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.750000, -0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, -0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.663414, -0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, -0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.556670, -0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, -0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.433013, -0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, -0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.296198, -0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, -0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.150384, -0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, -0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.883022, -0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, -0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.969846, -0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, -0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.939693, -0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, -0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.984808, -0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, -0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.969846, -0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, -0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.883022, -0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, -0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.813798, -0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, -0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.719846, -0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, -0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.604023, -0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, -0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.469846, -0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, -0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, -0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, -0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.163176, -0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, -0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.173648, 0.000000, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, 0.000000, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.342020, 0.000000, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, 0.000000, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.500000, 0.000000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, 0.000000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.642788, 0.000000, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, 0.000000, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.766044, 0.000000, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, 0.000000, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.866025, 0.000000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, 0.000000, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.939693, 0.000000, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, 0.000000, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.969846, -0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, -0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.984808, 0.000000, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, 0.000000, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.984808, -0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, -0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-1.000000, 0.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.969846, -0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, -0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.984808, 0.000000, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, 0.000000, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.925417, -0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, -0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.939693, 0.000000, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, 0.000000, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.852869, -0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, -0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.866025, 0.000000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, 0.000000, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.754407, -0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, -0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.766044, 0.000000, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, 0.000000, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.633022, -0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, -0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.642788, 0.000000, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, 0.000000, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.492404, -0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, -0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.500000, 0.000000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, 0.000000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.336824, -0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, -0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.342020, 0.000000, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, 0.000000, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, -0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, -0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.173648, 0.000000, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, 0.000000, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.173648, 0.000000, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, 0.000000, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.342020, 0.000000, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, 0.000000, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.500000, 0.000000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, 0.000000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.642788, 0.000000, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, 0.000000, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.766044, 0.000000, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, 0.000000, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.866025, 0.000000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, 0.000000, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.939693, 0.000000, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, 0.000000, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.984808, 0.000000, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, 0.000000, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.969846, 0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, 0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(-1.000000, 0.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-1.000000, 0.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.984808, 0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, 0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.984808, 0.000000, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, 0.000000, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.969846, 0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, 0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.939693, 0.000000, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, 0.000000, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.866025, 0.000000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, 0.000000, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.766044, 0.000000, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, 0.000000, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.642788, 0.000000, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, 0.000000, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.500000, 0.000000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, 0.000000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.342020, 0.000000, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, 0.000000, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.173648, 0.000000, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, 0.000000, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.883022, 0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, 0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.969846, 0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, 0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.984808, 0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.984808, 0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.939693, 0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, 0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.969846, 0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.969846, 0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.883022, 0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, 0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.750000, 0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, 0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.883022, 0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, 0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.939693, 0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.939693, 0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.866025, 0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, 0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.925417, 0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.925417, 0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.883022, 0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.883022, 0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.750000, 0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, 0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.586824, 0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, 0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.750000, 0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, 0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.866025, 0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.866025, 0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.766044, 0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, 0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.852869, 0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.852869, 0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.813798, 0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.813798, 0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.750000, 0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.750000, 0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.586824, 0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, 0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.413176, 0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, 0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.586824, 0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, 0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.766044, 0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.766044, 0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.642788, 0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, 0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.754407, 0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.754407, 0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.719846, 0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.719846, 0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.663414, 0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.663414, 0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.586824, 0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.586824, 0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.413176, 0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, 0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.250000, 0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, 0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.413176, 0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, 0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.642788, 0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.642788, 0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.500000, 0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, 0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.633022, 0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.633022, 0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.604023, 0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.604023, 0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.556670, 0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.556670, 0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.413176, 0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.413176, 0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.250000, 0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, 0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.116978, 0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, 0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.250000, 0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, 0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.500000, 0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.500000, 0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.342020, 0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, 0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.492404, 0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.492404, 0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.469846, 0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.469846, 0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.433013, 0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.433013, 0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.383022, 0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.383022, 0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.250000, 0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.250000, 0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.116978, 0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, 0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.030154, 0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, 0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(-0.116978, 0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, 0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.342020, 0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.342020, 0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.173648, 0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, 0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.336824, 0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.336824, 0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.321394, 0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.321394, 0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.296198, 0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.296198, 0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.262003, 0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.262003, 0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.219846, 0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.219846, 0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.116978, 0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.116978, 0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.030154, 0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, 0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(-0.030154, 0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, 0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.173648, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.173648, -0.984808)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(0.000000, 0.342020, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.342020, -0.939693)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(0.000000, 0.500000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.500000, -0.866025)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(0.000000, 0.642788, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.642788, -0.766044)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(0.000000, 0.766044, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.766044, -0.642788)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(0.000000, 0.866025, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.866025, -0.500000)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(0.000000, 0.939693, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.939693, -0.342020)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(0.000000, 0.984808, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.984808, -0.173648)),
TexturedColoredVertex(glm::vec3(-0.173648, 0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.173648, 0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(0.000000, 1.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 1.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(-0.171010, 0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.171010, 0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(0.000000, 0.984808, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.984808, 0.173648)),
TexturedColoredVertex(glm::vec3(-0.163176, 0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.163176, 0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(0.000000, 0.939693, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.939693, 0.342020)),
TexturedColoredVertex(glm::vec3(-0.150384, 0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.150384, 0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(0.000000, 0.866025, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.866025, 0.500000)),
TexturedColoredVertex(glm::vec3(-0.133022, 0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.133022, 0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(0.000000, 0.766044, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.766044, 0.642788)),
TexturedColoredVertex(glm::vec3(-0.111619, 0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.111619, 0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(0.000000, 0.642788, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.642788, 0.766044)),
TexturedColoredVertex(glm::vec3(-0.086824, 0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.086824, 0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(0.000000, 0.500000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.500000, 0.866025)),
TexturedColoredVertex(glm::vec3(-0.059391, 0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.059391, 0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(0.000000, 0.342020, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.342020, 0.939693)),
TexturedColoredVertex(glm::vec3(-0.030154, 0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(-0.030154, 0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.173648, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.173648, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.000000, 0.173648, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.173648, 0.984808)),
TexturedColoredVertex(glm::vec3(0.030154, 0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, 0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.342020, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.342020, 0.939693)),
TexturedColoredVertex(glm::vec3(0.059391, 0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(0.000000, 0.500000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.500000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.086824, 0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(0.000000, 0.642788, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.642788, 0.766044)),
TexturedColoredVertex(glm::vec3(0.111619, 0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(0.000000, 0.766044, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.766044, 0.642788)),
TexturedColoredVertex(glm::vec3(0.133022, 0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(0.000000, 0.866025, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.866025, 0.500000)),
TexturedColoredVertex(glm::vec3(0.150384, 0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(0.000000, 0.939693, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.939693, 0.342020)),
TexturedColoredVertex(glm::vec3(0.163176, 0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(0.000000, 0.984808, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.984808, 0.173648)),
TexturedColoredVertex(glm::vec3(0.171010, 0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(0.000000, 1.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 1.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.173648, 0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, 0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(0.000000, 0.984808, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.984808, -0.173648)),
TexturedColoredVertex(glm::vec3(0.171010, 0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(0.000000, 0.939693, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.939693, -0.342020)),
TexturedColoredVertex(glm::vec3(0.163176, 0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(0.000000, 0.866025, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.866025, -0.500000)),
TexturedColoredVertex(glm::vec3(0.150384, 0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(0.000000, 0.766044, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.766044, -0.642788)),
TexturedColoredVertex(glm::vec3(0.133022, 0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(0.000000, 0.642788, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.642788, -0.766044)),
TexturedColoredVertex(glm::vec3(0.111619, 0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(0.000000, 0.500000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.500000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.086824, 0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(0.000000, 0.342020, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.342020, -0.939693)),
TexturedColoredVertex(glm::vec3(0.059391, 0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(0.000000, 0.173648, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.173648, -0.984808)),
TexturedColoredVertex(glm::vec3(0.030154, 0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, 0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.030154, 0.171010, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, 0.171010, -0.984808)),
TexturedColoredVertex(glm::vec3(0.059391, 0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(0.059391, 0.336824, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.336824, -0.939693)),
TexturedColoredVertex(glm::vec3(0.116978, 0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, 0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(0.086824, 0.492404, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.492404, -0.866025)),
TexturedColoredVertex(glm::vec3(0.171010, 0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(0.111619, 0.633022, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.633022, -0.766044)),
TexturedColoredVertex(glm::vec3(0.219846, 0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(0.133022, 0.754407, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.754407, -0.642788)),
TexturedColoredVertex(glm::vec3(0.262003, 0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(0.150384, 0.852869, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.852869, -0.500000)),
TexturedColoredVertex(glm::vec3(0.296198, 0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(0.163176, 0.925417, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.925417, -0.342020)),
TexturedColoredVertex(glm::vec3(0.321394, 0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(0.171010, 0.969846, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.969846, -0.173648)),
TexturedColoredVertex(glm::vec3(0.336824, 0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(0.173648, 0.984808, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, 0.984808, 0.000000)),
TexturedColoredVertex(glm::vec3(0.342020, 0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, 0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(0.171010, 0.969846, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.969846, 0.173648)),
TexturedColoredVertex(glm::vec3(0.336824, 0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(0.163176, 0.925417, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.925417, 0.342020)),
TexturedColoredVertex(glm::vec3(0.321394, 0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(0.150384, 0.852869, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.852869, 0.500000)),
TexturedColoredVertex(glm::vec3(0.296198, 0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(0.133022, 0.754407, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.754407, 0.642788)),
TexturedColoredVertex(glm::vec3(0.262003, 0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(0.111619, 0.633022, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.633022, 0.766044)),
TexturedColoredVertex(glm::vec3(0.219846, 0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(0.086824, 0.492404, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.492404, 0.866025)),
TexturedColoredVertex(glm::vec3(0.171010, 0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(0.059391, 0.336824, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.336824, 0.939693)),
TexturedColoredVertex(glm::vec3(0.116978, 0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, 0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(0.030154, 0.171010, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.030154, 0.171010, 0.984808)),
TexturedColoredVertex(glm::vec3(0.059391, 0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.059391, 0.163176, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.163176, 0.984808)),
TexturedColoredVertex(glm::vec3(0.086824, 0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(0.116978, 0.321394, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, 0.321394, 0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, 0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, 0.469846, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.469846, 0.866025)),
TexturedColoredVertex(glm::vec3(0.250000, 0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, 0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(0.219846, 0.604023, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.604023, 0.766044)),
TexturedColoredVertex(glm::vec3(0.321394, 0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(0.262003, 0.719846, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.719846, 0.642788)),
TexturedColoredVertex(glm::vec3(0.383022, 0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(0.296198, 0.813798, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.813798, 0.500000)),
TexturedColoredVertex(glm::vec3(0.433013, 0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(0.321394, 0.883022, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.883022, 0.342020)),
TexturedColoredVertex(glm::vec3(0.469846, 0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(0.336824, 0.925417, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.925417, 0.173648)),
TexturedColoredVertex(glm::vec3(0.492404, 0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(0.342020, 0.939693, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, 0.939693, 0.000000)),
TexturedColoredVertex(glm::vec3(0.500000, 0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, 0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(0.336824, 0.925417, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.925417, -0.173648)),
TexturedColoredVertex(glm::vec3(0.492404, 0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(0.321394, 0.883022, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.883022, -0.342020)),
TexturedColoredVertex(glm::vec3(0.469846, 0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(0.296198, 0.813798, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.813798, -0.500000)),
TexturedColoredVertex(glm::vec3(0.433013, 0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(0.262003, 0.719846, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.719846, -0.642788)),
TexturedColoredVertex(glm::vec3(0.383022, 0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(0.219846, 0.604023, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.604023, -0.766044)),
TexturedColoredVertex(glm::vec3(0.321394, 0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(0.171010, 0.469846, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.469846, -0.866025)),
TexturedColoredVertex(glm::vec3(0.250000, 0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, 0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(0.116978, 0.321394, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.116978, 0.321394, -0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, 0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(0.059391, 0.163176, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.059391, 0.163176, -0.984808)),
TexturedColoredVertex(glm::vec3(0.086824, 0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.086824, 0.150384, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.150384, -0.984808)),
TexturedColoredVertex(glm::vec3(0.111619, 0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(0.171010, 0.296198, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.296198, -0.939693)),
TexturedColoredVertex(glm::vec3(0.219846, 0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(0.250000, 0.433013, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, 0.433013, -0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, 0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, 0.556670, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.556670, -0.766044)),
TexturedColoredVertex(glm::vec3(0.413176, 0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, 0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(0.383022, 0.663414, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.663414, -0.642788)),
TexturedColoredVertex(glm::vec3(0.492404, 0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(0.433013, 0.750000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.750000, -0.500000)),
TexturedColoredVertex(glm::vec3(0.556670, 0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(0.469846, 0.813798, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.813798, -0.342020)),
TexturedColoredVertex(glm::vec3(0.604023, 0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.492404, 0.852869, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.852869, -0.173648)),
TexturedColoredVertex(glm::vec3(0.633022, 0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(0.500000, 0.866025, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, 0.866025, 0.000000)),
TexturedColoredVertex(glm::vec3(0.642788, 0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, 0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(0.492404, 0.852869, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.852869, 0.173648)),
TexturedColoredVertex(glm::vec3(0.633022, 0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(0.469846, 0.813798, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.813798, 0.342020)),
TexturedColoredVertex(glm::vec3(0.604023, 0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.433013, 0.750000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.750000, 0.500000)),
TexturedColoredVertex(glm::vec3(0.556670, 0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(0.383022, 0.663414, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.663414, 0.642788)),
TexturedColoredVertex(glm::vec3(0.492404, 0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(0.321394, 0.556670, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.556670, 0.766044)),
TexturedColoredVertex(glm::vec3(0.413176, 0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, 0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(0.250000, 0.433013, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.250000, 0.433013, 0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, 0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(0.171010, 0.296198, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.296198, 0.939693)),
TexturedColoredVertex(glm::vec3(0.219846, 0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(0.086824, 0.150384, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.086824, 0.150384, 0.984808)),
TexturedColoredVertex(glm::vec3(0.111619, 0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.111619, 0.133022, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.133022, 0.984808)),
TexturedColoredVertex(glm::vec3(0.133022, 0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(0.219846, 0.262003, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.262003, 0.939693)),
TexturedColoredVertex(glm::vec3(0.262003, 0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(0.321394, 0.383022, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.383022, 0.866025)),
TexturedColoredVertex(glm::vec3(0.383022, 0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(0.413176, 0.492404, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, 0.492404, 0.766045)),
TexturedColoredVertex(glm::vec3(0.492404, 0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(0.492404, 0.586824, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.586824, 0.642788)),
TexturedColoredVertex(glm::vec3(0.586824, 0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, 0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(0.556670, 0.663414, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.663414, 0.500000)),
TexturedColoredVertex(glm::vec3(0.663414, 0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(0.604023, 0.719846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.719846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.719846, 0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(0.633022, 0.754407, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.754407, 0.173648)),
TexturedColoredVertex(glm::vec3(0.754407, 0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(0.642788, 0.766044, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, 0.766044, 0.000000)),
TexturedColoredVertex(glm::vec3(0.766044, 0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, 0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(0.633022, 0.754407, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.754407, -0.173648)),
TexturedColoredVertex(glm::vec3(0.754407, 0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(0.604023, 0.719846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.719846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.719846, 0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(0.556670, 0.663414, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.663414, -0.500000)),
TexturedColoredVertex(glm::vec3(0.663414, 0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(0.492404, 0.586824, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.586824, -0.642788)),
TexturedColoredVertex(glm::vec3(0.586824, 0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, 0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(0.413176, 0.492404, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.413176, 0.492404, -0.766045)),
TexturedColoredVertex(glm::vec3(0.492404, 0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(0.321394, 0.383022, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.383022, -0.866025)),
TexturedColoredVertex(glm::vec3(0.383022, 0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(0.219846, 0.262003, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.219846, 0.262003, -0.939693)),
TexturedColoredVertex(glm::vec3(0.262003, 0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(0.111619, 0.133022, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.111619, 0.133022, -0.984808)),
TexturedColoredVertex(glm::vec3(0.133022, 0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.133022, 0.111619, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.111619, -0.984808)),
TexturedColoredVertex(glm::vec3(0.150384, 0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(0.262003, 0.219846, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.219846, -0.939693)),
TexturedColoredVertex(glm::vec3(0.296198, 0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(0.383022, 0.321394, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.321394, -0.866025)),
TexturedColoredVertex(glm::vec3(0.433013, 0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.492404, 0.413176, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.413176, -0.766045)),
TexturedColoredVertex(glm::vec3(0.556670, 0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(0.586824, 0.492404, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, 0.492404, -0.642788)),
TexturedColoredVertex(glm::vec3(0.663414, 0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.663414, 0.556670, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.556670, -0.500000)),
TexturedColoredVertex(glm::vec3(0.750000, 0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, 0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(0.719846, 0.604023, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.604023, -0.342020)),
TexturedColoredVertex(glm::vec3(0.813798, 0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.754407, 0.633022, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.633022, -0.173648)),
TexturedColoredVertex(glm::vec3(0.852869, 0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(0.766044, 0.642788, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, 0.642788, 0.000000)),
TexturedColoredVertex(glm::vec3(0.866025, 0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, 0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.754407, 0.633022, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.633022, 0.173648)),
TexturedColoredVertex(glm::vec3(0.852869, 0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(0.719846, 0.604023, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.604023, 0.342020)),
TexturedColoredVertex(glm::vec3(0.813798, 0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.663414, 0.556670, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.556670, 0.500000)),
TexturedColoredVertex(glm::vec3(0.750000, 0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, 0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(0.586824, 0.492404, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.586824, 0.492404, 0.642788)),
TexturedColoredVertex(glm::vec3(0.663414, 0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.492404, 0.413176, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.413176, 0.766045)),
TexturedColoredVertex(glm::vec3(0.556670, 0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(0.383022, 0.321394, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.383022, 0.321394, 0.866025)),
TexturedColoredVertex(glm::vec3(0.433013, 0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.262003, 0.219846, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.262003, 0.219846, 0.939693)),
TexturedColoredVertex(glm::vec3(0.296198, 0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(0.133022, 0.111619, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.133022, 0.111619, 0.984808)),
TexturedColoredVertex(glm::vec3(0.150384, 0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.150384, 0.086824, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.086824, 0.984808)),
TexturedColoredVertex(glm::vec3(0.163176, 0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(0.296198, 0.171010, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.171010, 0.939693)),
TexturedColoredVertex(glm::vec3(0.321394, 0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(0.433013, 0.250000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.250000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.469846, 0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(0.556670, 0.321394, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.321394, 0.766044)),
TexturedColoredVertex(glm::vec3(0.604023, 0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(0.663414, 0.383022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.383022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.719846, 0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(0.750000, 0.433013, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, 0.433013, 0.500000)),
TexturedColoredVertex(glm::vec3(0.813798, 0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(0.813798, 0.469846, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.469846, 0.342020)),
TexturedColoredVertex(glm::vec3(0.883022, 0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, 0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(0.852869, 0.492404, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.492404, 0.173648)),
TexturedColoredVertex(glm::vec3(0.925417, 0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(0.866025, 0.500000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, 0.500000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.939693, 0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, 0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(0.852869, 0.492404, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.492404, -0.173648)),
TexturedColoredVertex(glm::vec3(0.925417, 0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(0.813798, 0.469846, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.469846, -0.342020)),
TexturedColoredVertex(glm::vec3(0.883022, 0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, 0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(0.750000, 0.433013, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.750000, 0.433013, -0.500000)),
TexturedColoredVertex(glm::vec3(0.813798, 0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(0.663414, 0.383022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.663414, 0.383022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.719846, 0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(0.556670, 0.321394, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.556670, 0.321394, -0.766044)),
TexturedColoredVertex(glm::vec3(0.604023, 0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(0.433013, 0.250000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.433013, 0.250000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.469846, 0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(0.296198, 0.171010, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.296198, 0.171010, -0.939693)),
TexturedColoredVertex(glm::vec3(0.321394, 0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(0.150384, 0.086824, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.150384, 0.086824, -0.984808)),
TexturedColoredVertex(glm::vec3(0.163176, 0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000)),
TexturedColoredVertex(glm::vec3(0.163176, 0.059391, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.059391, -0.984808)),
TexturedColoredVertex(glm::vec3(0.171010, 0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(0.321394, 0.116978, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.116978, -0.939693)),
TexturedColoredVertex(glm::vec3(0.336824, 0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(0.469846, 0.171010, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.171010, -0.866025)),
TexturedColoredVertex(glm::vec3(0.492404, 0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(0.604023, 0.219846, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.219846, -0.766044)),
TexturedColoredVertex(glm::vec3(0.633022, 0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(0.719846, 0.262003, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.262003, -0.642788)),
TexturedColoredVertex(glm::vec3(0.754407, 0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.813798, 0.296198, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.296198, -0.500000)),
TexturedColoredVertex(glm::vec3(0.852869, 0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(0.883022, 0.321394, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, 0.321394, -0.342020)),
TexturedColoredVertex(glm::vec3(0.925417, 0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(0.925417, 0.336824, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.336824, -0.173648)),
TexturedColoredVertex(glm::vec3(0.969846, 0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, 0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(0.939693, 0.342020, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, 0.342020, 0.000000)),
TexturedColoredVertex(glm::vec3(0.984808, 0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, 0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(0.925417, 0.336824, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.336824, 0.173648)),
TexturedColoredVertex(glm::vec3(0.969846, 0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, 0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(0.883022, 0.321394, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.883022, 0.321394, 0.342020)),
TexturedColoredVertex(glm::vec3(0.925417, 0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(0.813798, 0.296198, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.813798, 0.296198, 0.500000)),
TexturedColoredVertex(glm::vec3(0.852869, 0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(0.719846, 0.262003, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.719846, 0.262003, 0.642788)),
TexturedColoredVertex(glm::vec3(0.754407, 0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.604023, 0.219846, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.604023, 0.219846, 0.766044)),
TexturedColoredVertex(glm::vec3(0.633022, 0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(0.469846, 0.171010, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.469846, 0.171010, 0.866025)),
TexturedColoredVertex(glm::vec3(0.492404, 0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(0.321394, 0.116978, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.321394, 0.116978, 0.939693)),
TexturedColoredVertex(glm::vec3(0.336824, 0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(0.163176, 0.059391, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.163176, 0.059391, 0.984808)),
TexturedColoredVertex(glm::vec3(0.171010, 0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(0.000000, 0.000000, 1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, 1.000000)),
TexturedColoredVertex(glm::vec3(0.171010, 0.030154, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.030154, 0.984808)),
TexturedColoredVertex(glm::vec3(0.173648, 0.000000, 0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, 0.000000, 0.984808)),
TexturedColoredVertex(glm::vec3(0.336824, 0.059391, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.059391, 0.939693)),
TexturedColoredVertex(glm::vec3(0.342020, 0.000000, 0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, 0.000000, 0.939693)),
TexturedColoredVertex(glm::vec3(0.492404, 0.086824, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.086824, 0.866025)),
TexturedColoredVertex(glm::vec3(0.500000, 0.000000, 0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, 0.000000, 0.866025)),
TexturedColoredVertex(glm::vec3(0.633022, 0.111619, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.111619, 0.766044)),
TexturedColoredVertex(glm::vec3(0.642788, 0.000000, 0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, 0.000000, 0.766044)),
TexturedColoredVertex(glm::vec3(0.754407, 0.133022, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.133022, 0.642788)),
TexturedColoredVertex(glm::vec3(0.766044, 0.000000, 0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, 0.000000, 0.642788)),
TexturedColoredVertex(glm::vec3(0.852869, 0.150384, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.150384, 0.500000)),
TexturedColoredVertex(glm::vec3(0.866025, 0.000000, 0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, 0.000000, 0.500000)),
TexturedColoredVertex(glm::vec3(0.925417, 0.163176, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.163176, 0.342020)),
TexturedColoredVertex(glm::vec3(0.939693, 0.000000, 0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, 0.000000, 0.342020)),
TexturedColoredVertex(glm::vec3(0.969846, 0.171010, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, 0.171010, 0.173648)),
TexturedColoredVertex(glm::vec3(0.984808, 0.000000, 0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, 0.000000, 0.173648)),
TexturedColoredVertex(glm::vec3(0.984808, 0.173648, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, 0.173648, 0.000000)),
TexturedColoredVertex(glm::vec3(1.000000, 0.000000, 0.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(1.000000, 0.000000, 0.000000)),
TexturedColoredVertex(glm::vec3(0.969846, 0.171010, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.969846, 0.171010, -0.173648)),
TexturedColoredVertex(glm::vec3(0.984808, 0.000000, -0.173648), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.984808, 0.000000, -0.173648)),
TexturedColoredVertex(glm::vec3(0.925417, 0.163176, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.925417, 0.163176, -0.342020)),
TexturedColoredVertex(glm::vec3(0.939693, 0.000000, -0.342020), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.939693, 0.000000, -0.342020)),
TexturedColoredVertex(glm::vec3(0.852869, 0.150384, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.852869, 0.150384, -0.500000)),
TexturedColoredVertex(glm::vec3(0.866025, 0.000000, -0.500000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.866025, 0.000000, -0.500000)),
TexturedColoredVertex(glm::vec3(0.754407, 0.133022, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.754407, 0.133022, -0.642788)),
TexturedColoredVertex(glm::vec3(0.766044, 0.000000, -0.642788), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.766044, 0.000000, -0.642788)),
TexturedColoredVertex(glm::vec3(0.633022, 0.111619, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.633022, 0.111619, -0.766044)),
TexturedColoredVertex(glm::vec3(0.642788, 0.000000, -0.766044), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.642788, 0.000000, -0.766044)),
TexturedColoredVertex(glm::vec3(0.492404, 0.086824, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.492404, 0.086824, -0.866025)),
TexturedColoredVertex(glm::vec3(0.500000, 0.000000, -0.866025), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.500000, 0.000000, -0.866025)),
TexturedColoredVertex(glm::vec3(0.336824, 0.059391, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.336824, 0.059391, -0.939693)),
TexturedColoredVertex(glm::vec3(0.342020, 0.000000, -0.939693), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.342020, 0.000000, -0.939693)),
TexturedColoredVertex(glm::vec3(0.171010, 0.030154, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.171010, 0.030154, -0.984808)),
TexturedColoredVertex(glm::vec3(0.173648, 0.000000, -0.984808), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.173648, 0.000000, -0.984808)),
TexturedColoredVertex(glm:: vec3(0.000000, 0.000000, -1.000000), glm::vec3(1.0f,1.0f,1.0f), glm::vec2(0.0f,0.0f), glm:: vec3(0.000000, 0.000000, -1.000000))
};

TexturedColoredVertex snowFloorArray[] = {
	TexturedColoredVertex(glm::vec3(-1.0*unit,0.0f, 1.0f*unit), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec2(20.0f, 20.0f), glm::vec3(0.0f,1.0f,0.0f)),
	TexturedColoredVertex(glm::vec3( 1.0*unit,0.0f, 1.0f*unit), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec2(20.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f)),
	TexturedColoredVertex(glm::vec3( 1.0*unit,0.0f,-1.0f*unit), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec2(0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f)),
	
	TexturedColoredVertex(glm::vec3(-1.0*unit,0.0f, 1.0*unit), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec2(20.0f,20.0f), glm::vec3(0.0f,1.0f,0.0f)),
	TexturedColoredVertex(glm::vec3( 1.0*unit,0.0f,-1.0*unit), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0*unit,0.0f,-1.0*unit), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec2(0.0f,20.0f), glm::vec3(0.0f,1.0f,0.0f)),
};

TexturedColoredVertex unitConeArray[] = {
		TexturedColoredVertex(glm::vec3(unit,0.0f,unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::cross((glm::vec3(unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(0.0f,unit,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 1.0f), glm::cross((glm::vec3(unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(unit,0.0f,-unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::cross((glm::vec3(unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)))),
		
		TexturedColoredVertex(glm::vec3(unit,0.0f,-unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::cross((glm::vec3(unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(-unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(0.0f,unit,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 1.0f), glm::cross((glm::vec3(unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(-unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(-unit,0.0f,-unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::cross((glm::vec3(unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(-unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)))),
		
		TexturedColoredVertex(glm::vec3(-unit,0.0f,-unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::cross((glm::vec3(-unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(-unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(0.0f,unit,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 1.0f), glm::cross((glm::vec3(-unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(-unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(-unit,0.0f,unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::cross((glm::vec3(-unit,0.0f,-unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(-unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)))),
		
		TexturedColoredVertex(glm::vec3(-unit,0.0f,unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::cross((glm::vec3(-unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(0.0f,unit,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 1.0f), glm::cross((glm::vec3(-unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)))),
		TexturedColoredVertex(glm::vec3(unit,0.0f,unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::cross((glm::vec3(-unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f)),(glm::vec3(unit,0.0f,unit)-glm::vec3(0.0f,unit,0.0f))))	
};

//Holds the coordinates/colours for triangles that form a cube. Adapted from Lab03 code.
const TexturedColoredVertex unitCubeArray[] = {
		TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,0.0f,0.0f)),
        
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,0.0f,0.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,0.0f,1.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,0.0f,1.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,0.0f,1.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,0.0f,1.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,0.0f,1.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,0.0f,1.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,0.0f,0.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,0.0f,0.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit,-0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,0.0f,0.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f)),
        
        TexturedColoredVertex(glm::vec3( 0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit,-0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,1.0f,0.0f)),
        TexturedColoredVertex(glm::vec3(-0.5*unit, 0.5*unit, 0.5*unit), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,1.0f,0.0f))
};
//2.0f


void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}



void SetUniformMat4(GLuint shader_id, const char* uniform_name, glm::mat4 uniform_value)
{
  glUseProgram(shader_id);
  glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &uniform_value[0][0]);
}

void SetUniformVec3(GLuint shader_id, const char* uniform_name, glm::vec3 uniform_value)
{
  glUseProgram(shader_id);
  glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1, glm::value_ptr(uniform_value));
}

void SetUniform1ui(GLuint shader_id, const char* uniform_name, unsigned int uniform_value)
{
  glUseProgram(shader_id);
  glUniform1ui(glGetUniformLocation(shader_id, uniform_name), uniform_value);
}

void SetUniform1f(GLuint shader_id, const char* uniform_name, float uniform_value)
{
  glUseProgram(shader_id);
  glUniform1f(glGetUniformLocation(shader_id, uniform_name), uniform_value);
  glUseProgram(0);
}


int createVertexArrayObject(const glm::vec3* vertexArray, int arraySize)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          2*sizeof(glm::vec3),
                          (void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return vertexArrayObject;
}

int createTexturedVertexArrayObject(const TexturedColoredVertex* vertexArray, int arraySize)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          sizeof(TexturedColoredVertex), //2*sizeof(glm::vec3)+sizeof(glm::vec2), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex), //2*sizeof(glm::vec3)+sizeof(glm::vec2),
                          (void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);
	
	
	glVertexAttribPointer(2,                            // attribute 2 matches aColor in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex), //2*sizeof(glm::vec2)+sizeof(glm::vec2),
                          (void*)(2*sizeof(glm::vec3))      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(3,                            // attribute 2 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex), //2*sizeof(glm::vec2)+sizeof(glm::vec2),
                          (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2))      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return vertexArrayObject;
}

GLuint loadtext(char* filename)
{
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	assert(textureID != 0);
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//hardcoded size values
	int width = 256;
	int height = 256;
	int numberofchannels = 3;
	unsigned char *data = stbi_load(filename, &width, &height, &numberofchannels, 0);
	
	  if (!data)
	  {
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	  }
	
	  // Step4 Upload the texture to the PU
	  GLenum format = 0;
	  if (numberofchannels == 1)
		  format = GL_RED;
	  else if (numberofchannels == 3)
		  format = GL_RGB;
	  else if (numberofchannels == 4)
		  format = GL_RGBA;
	  
	  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	  glGenerateMipmap(GL_TEXTURE_2D);
	
	  // Step5 Free resources
	  stbi_image_free(data);
	  glBindTexture(GL_TEXTURE_2D, 0);
	  
	  return textureID;
}


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024 x 768
    GLFWwindow* window = glfwCreateWindow(1024, 768, "COMP371 Quiz 2", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
	
	// Culling is disabled, otherwise there would be display issues because of the transparent front faces of the letters.
	//glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	
    //Variables that keep track of the cursor position
	double prevXpos = 0.0;
	double prevYpos = 0.0;
	double currXpos = 0.0;
	double currYpos = 0.0;
    
	
	//load textures
	GLuint carpetTextureID = loadtext("../assets/textures/carpet.png");
	GLuint woodTextureID = loadtext("../assets/textures/wood.png"); 
	GLuint glossTextureID = loadtext("../assets/textures/gloss.png");
	GLuint carrotTextureID = loadtext("../assets/textures/water.png");
	
	//Load shaders
	std::string shaderPathPrefix = "../assets/shaders/";
	GLuint textureShader = loadSHADER(shaderPathPrefix + "texturevertex.glsl", shaderPathPrefix + "texturefragment.glsl");
	GLuint shadowShader = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");
	
	
	// Vectors used for view matrix by default
	glm::vec3 eye(0.0f,1.0f,0.0f);
    glm::vec3 center(-1.0f, 1.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	
	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);	
		
	glm::mat4 projectionMatrix = glm::perspective(glm::radians( 85.0f ),  // field of view in degrees 
                                                  1024.0f / 768.0f,      // aspect ratio
                                                  0.01f, 100.0f);
														  
														  
		setViewMatrix(textureShader, viewMatrix);

		setProjectionMatrix(textureShader, projectionMatrix);
	
    
	
	// Set up texture and framebuffer for shadow map
	// Adapted from lab 08 code
	// This sets up the groundwork for the first pass by creating a texture and framebuffer
	// that will be used to store depth information.
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
    
    // Define and upload geometry to the GPU here ...
    int cubeAO = createTexturedVertexArrayObject(unitCubeArray, sizeof(unitCubeArray));
	int floorAO = createTexturedVertexArrayObject(snowFloorArray, sizeof(snowFloorArray));
	int sphereAO = createTexturedVertexArrayObject(unitSphereArray, sizeof(unitSphereArray));
	


	
	//Variables used to toggle lights
	int lastZstate = GLFW_RELEASE;
	int lastXstate = GLFW_RELEASE;
	int lastCstate = GLFW_RELEASE;
	
	int lastVstate = GLFW_RELEASE;
	int lastBstate = GLFW_RELEASE;
	int lastNstate = GLFW_RELEASE;
	
	bool enableR = true;
	bool enableG = true;
	bool enableB = true;
	
	float rcomp = 1.0;
	float gcomp = 1.0;
	float bcomp = 1.0;
	
	bool redlens = false;
	bool greenlens = false;
	bool bluelens = false;
	
	//Variables used to keep track of movement
	float horizmov = 0.0;
	float vertmov = 0.0;
	
	//uniforms that change the colours to use in the shader
	GLuint colormodeLocationT = glGetUniformLocation(textureShader, "colorMode");
	

	
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
		
		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		
		
		
		// Each frame, reset color of each pixel to glClearColor and reset depth bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Dark grey background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		//set up textures
		
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, carpetTextureID);

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, woodTextureID);
		
		glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, glossTextureID);
		
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, carrotTextureID);
		
		
		GLuint textureLocation = glGetUniformLocation(textureShader, "CarpetSampler");
       	GLuint textureLocation2 = glGetUniformLocation(textureShader, "WoodSampler");

		GLuint textureLocation3 = glGetUniformLocation(textureShader, "GlossSampler");

		GLuint textureLocation4 = glGetUniformLocation(textureShader, "shadowmap");
		
		GLuint textureLocation5 = glGetUniformLocation(textureShader, "CarrotSampler");
		
		
        glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0
        glUniform1i(textureLocation2, 1);
		glUniform1i(textureLocation3, 2);
		glUniform1i(textureLocation4, 3);
		glUniform1i(textureLocation5, 4);
		
		
		
		
		// light parameters
		//adapted from lab08 code
		glm::vec3 lightPosition = glm::vec3(unit*-10.0f,unit*0.0f,unit*0.0f); //glm::vec3(0.0f,unit*5.0f,unit*30.0f); // the location of the light in 3D space, set light to be 30 units above the origin
		glm::vec3 lightFocus(unit*-20.0f, unit*10.0f, 0.0);      // the point in 3D space the light "looks" at
		glm::vec3 lightDirection = normalize(lightFocus - lightPosition);
		
		
		glUseProgram(shadowShader);
		
		//light parameters
		float lightAngleOuter = 40.0;
		float lightAngleInner = 20.0;
		float lightNearPlane = 2.0f;
		float lightFarPlane = 180.0f;
		
		//determines light colour as per toggles
		if(enableR) { rcomp = 1.0; }
		if(!enableR) { rcomp = 0.0; }
		
		if(enableG) { gcomp = 1.0; }
		if(!enableG) { gcomp = 0.0; }
		
		if(enableB) { bcomp = 1.0; }
		if(!enableB) { bcomp = 0.0; }
			
		
		//colour of the side spotlight
		glm::vec3 lightcolor(rcomp,gcomp,bcomp);

	
		//set up matrices from the light's perspective
		glm::mat4 lightProjectionMatrix = glm::perspective(85.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, lightNearPlane, lightFarPlane);
		glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
		
		
		
		//upload to shader
		
		SetUniformMat4(shadowShader, "lightspacematrix", lightSpaceMatrix);
		SetUniformMat4(textureShader, "lightspacematrix", lightSpaceMatrix);
		
		
		SetUniform1f(textureShader, "light_cutoff_outer", std::cos(glm::radians(lightAngleInner)));
		SetUniform1f(textureShader, "light_cutoff_inner", std::cos(glm::radians(lightAngleOuter)));
		
		SetUniformVec3(textureShader, "lightcolor", lightcolor);
        SetUniformVec3(textureShader, "lightposition", lightPosition);
		SetUniformVec3(textureShader, "lightdirection", lightDirection);
		
		
		

		

		//FIRST PASS - RENDER DEPTH MAP
		
		
		
		glUseProgram(shadowShader);
		
		SetUniformMat4(shadowShader, "lightspacematrix", lightSpaceMatrix);
		SetUniformMat4(textureShader, "lightspacematrix", lightSpaceMatrix);
		
		
		 // Use proper image output size
		  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		  // Bind depth map texture as output framebuffer
		  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		  // Clear depth data on the framebuffer
		  glClear(GL_DEPTH_BUFFER_BIT);
		  //glBindTexture(GL_TEXTURE_2D, depthMap);

		
		
		
		
		glm::mat4 floorWorldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
		
		//Set up snow texture over floor grid
		glUseProgram(shadowShader);
		glBindVertexArray(floorAO);
		
		setWorldMatrix(shadowShader, floorWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		
		


		
		
		
		
		
		glBindVertexArray(cubeAO);
		//:::::::::::::::::::::SET UP WORLD CUBE
		
		//Invert the cube inside-out when scaling so that it doesn't disappear because of backface culling
		glm::mat4 skyWorldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f));
		setWorldMatrix(shadowShader, skyWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		
	
		
		
		
		glBindVertexArray(sphereAO);
		glm::mat4 sphereWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, vertmov, horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f*unit, unit*10.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		setWorldMatrix(shadowShader, sphereWorldMatrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 1261);
		
		
		
		glBindVertexArray(cubeAO);
		glm::mat4 cubeWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.7*vertmov, 0.7*horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-22.0f*unit, 10.0f*unit, -6.0f*unit)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 2.5f, 2.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		setWorldMatrix(shadowShader, cubeWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		cubeWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.7*vertmov, 0.7*horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-22.0f*unit, 6.0f*unit, -15.0f*unit)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, 3.5f, 3.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		setWorldMatrix(shadowShader, cubeWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		cubeWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.7*vertmov, 0.7*horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-21.0f*unit, 8.0f*unit, 14.0f*unit)) * glm::scale(glm::mat4(1.0f), glm::vec3(5.5f, 5.5f, 5.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		setWorldMatrix(shadowShader, cubeWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		
		
		
		
		
		
		
		//SECOND PASS - Reset viewport and everything else back, and render as normal
		
		//int fbwidth, fbheight;
	    //glfwGetFramebufferSize(window, &fbwidth, &fbheight);
		
		
		glViewport(0, 0, 1024.0f, 768.0f);	//hardcoded values to avoid stretching
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Each frame, reset color of each pixel to glClearColor and reset depth bits
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		
		floorWorldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
		
		//Set up snow texture over floor grid
		glUseProgram(textureShader);
		glBindVertexArray(floorAO);
		
		glUniform1ui(colormodeLocationT, 0);
		setWorldMatrix(textureShader, floorWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		
		


		
		
		
		
		
		glBindVertexArray(cubeAO);
		//:::::::::::::::::::::SET UP WOODEN ROOM
		
		//Set color mode to sky texture
		glUniform1ui(colormodeLocationT, 1);
		
		//Invert the cube inside-out when scaling so that it doesn't disappear because of backface culling
		skyWorldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f));
		setWorldMatrix(textureShader, skyWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		
	
		
		
		//set up sphere
		glBindVertexArray(sphereAO);
		glUniform1ui(colormodeLocationT, 2);
		sphereWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, vertmov, horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f*unit, unit*10.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		setWorldMatrix(textureShader, sphereWorldMatrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 1261);
		
		
		//set up cubes
		glBindVertexArray(cubeAO);
		glUniform1ui(colormodeLocationT, 3);
		cubeWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.7*vertmov, 0.7*horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-22.0f*unit, 10.0f*unit, -6.0f*unit)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 2.5f, 2.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		setWorldMatrix(textureShader, cubeWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glUniform1ui(colormodeLocationT, 0);
		cubeWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.7*vertmov, 0.7*horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-22.0f*unit, 6.0f*unit, -15.0f*unit)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, 3.5f, 3.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		setWorldMatrix(textureShader, cubeWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glUniform1ui(colormodeLocationT, 4);
		cubeWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.7*vertmov, 0.7*horizmov)) * glm::translate(glm::mat4(1.0f), glm::vec3(-21.0f*unit, 8.0f*unit, 14.0f*unit)) * glm::scale(glm::mat4(1.0f), glm::vec3(5.5f, 5.5f, 5.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		setWorldMatrix(textureShader, cubeWorldMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		
		
		
		
		//set up lenses
		glBindVertexArray(cubeAO);
		
		
		glUniform1ui(colormodeLocationT, 7);
		glm::mat4 lensWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f*unit, 10.0f*unit, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		setWorldMatrix(textureShader, lensWorldMatrix);
		if(redlens){
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
		
		glUniform1ui(colormodeLocationT, 6);
		lensWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f*unit, 10.0f*unit, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		setWorldMatrix(textureShader, lensWorldMatrix);
		if(greenlens){
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
		
		glUniform1ui(colormodeLocationT, 5);
		lensWorldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f*unit, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		setWorldMatrix(textureShader, lensWorldMatrix);
		if(bluelens){
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
		
		glBindVertexArray(0);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		

        // End frame
        glfwSwapBuffers(window);
        
        // Detect inputs
        glfwPollEvents();
        
		//::::::::::::::::::DISPLAY OPTIONS (debug)
		//Set to point mode if P is pressed
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); }
		
		//Set to line mode if L is pressed
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		
		//Set to triangle mode if T is pressed
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
		
		
		
		//::::::::::::::::::MOVEMENT OPTIONS
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { vertmov += 0.001; }
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { vertmov -= 0.001; }
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { horizmov -= 0.001; }
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { horizmov += 0.001; }
		
		
		
		////::::::::::::::::::LIGHT TOGGLES
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && lastZstate == GLFW_RELEASE)
		{
			if(enableR)
			{
				enableR = false;
			}
			else
			{
				enableR = true;
			}
		}
		lastZstate = glfwGetKey(window, GLFW_KEY_Z);
		
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && lastXstate == GLFW_RELEASE)
		{
			if(enableG)
			{
				enableG = false;
			}
			else
			{
				enableG = true;
			}
		}
		lastXstate = glfwGetKey(window, GLFW_KEY_X);
		
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && lastCstate == GLFW_RELEASE)
		{
			if(enableB)
			{
				enableB = false;
			}
			else
			{
				enableB = true;
			}
		}
		lastCstate = glfwGetKey(window, GLFW_KEY_C);
		
		
		//:::::::::::::::::::::::LENS TOGGLES
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && lastVstate == GLFW_RELEASE)
		{
			if(redlens)
			{
				redlens = false;
			}
			else
			{
				redlens = true;
			}
		}
		lastVstate = glfwGetKey(window, GLFW_KEY_V);
		
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && lastBstate == GLFW_RELEASE)
		{
			if(greenlens)
			{
				greenlens = false;
			}
			else
			{
				greenlens = true;
			}
		}
		lastBstate = glfwGetKey(window, GLFW_KEY_B);
		
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && lastNstate == GLFW_RELEASE)
		{
			if(bluelens)
			{
				bluelens = false;
			}
			else
			{
				bluelens = true;
			}
		}
		lastNstate = glfwGetKey(window, GLFW_KEY_N);
		
		
		//::::::::::::::::::CAMERA OPTIONS (debug)
		
		//General logic is as follows:
		//We get the cursor position at the start and end, so we can compare how the cursor is moving.
		//We perform these comparisons and adjust certain camera values accordingly to pan/tilt/zoom as desired.
		
		//Get the cursor position
		glfwGetCursorPos(window, &currXpos, &currYpos);
		
		//Tilt
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			if (currYpos > prevYpos)
			{
				center += glm::vec3(0.0f,0.7f,0.0f);
			}
			
			if (currYpos < prevYpos)
			{
				center -= glm::vec3(0.0f,0.7f,0.0f);
			}
		}
		
		
		//Pan
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			if (currXpos > prevXpos)
			{
				center += glm::normalize(glm::cross(up,center-eye));
			}
			
			if (currXpos < prevXpos)
			{
				center -= glm::normalize(glm::cross(up,center-eye));
			}
		}
		
		
		//Make the current coordinates the previous ones for the next iteration.
		prevXpos = currXpos;
		prevYpos = currYpos;
		
		
		
		
		viewMatrix = glm::lookAt(eye, center, up);	
		
		SetUniformVec3(textureShader, "viewposition", eye);
	
		setViewMatrix(textureShader, viewMatrix);
		
		glm::mat4 projectionMatrix = glm::perspective(glm::radians( 85.0f ),  // field of view in degrees 
                                                          1024.0f / 768.0f,      // aspect ratio
                                                          0.01f, 100.0f);
		
		setProjectionMatrix(textureShader, projectionMatrix);
	
		
    }
    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}

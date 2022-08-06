#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,

using namespace std;

int loadSHADER(string vertex_file_path, string fragment_file_path);

#endif
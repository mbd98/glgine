cmake_minimum_required(VERSION 3.16.3)
project(COMP371_Project)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

find_package(GLEW REQUIRED)
include_directories(include irrKlang-64bit-1.6.0/include imgui)


add_library(glad glad.c)


aux_source_directory(imgui IMGUI)
add_library(ImGuiLib ${IMGUI})

include_directories(src/ )
aux_source_directory(src SOURCE)
link_directories(irrKlang-64bit-1.6.0/bin/linux-gcc-64)
add_executable(project main.cpp ${SOURCE})
target_link_libraries(project ImGuiLib glfw OpenGL::GL GLEW::GLEW fftw3 libIrrKlang.so ${CMAKE_DL_LIBS})

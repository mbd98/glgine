cmake_minimum_required(VERSION 3.16.3)
project(COMP371_Project)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(BuildGLEW)
include(BuildGLFW)
include(BuildGLM)


set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(GLEW_STATIC=1 GLM_FORCE_RADIANS=1)

find_package(GLEW REQUIRED)
include_directories(src irrKlang-64bit-1.6.0/include imgui)

aux_source_directory(imgui IMGUI)
add_library(ImGuiLib ${IMGUI})

aux_source_directory(src SOURCE)
link_directories(irrKlang-64bit-1.6.0/bin/linux-gcc-64)
add_executable(project main.cpp ${SOURCE})
#target_link_libraries(project ImGuiLib glfw OpenGL::GL GLEW::GLEW fftw3 libIrrKlang.so ${CMAKE_DL_LIBS})
target_link_libraries(project ImGuiLib glfw OpenGL::GL glew_s glfw glm ${CMAKE_DL_LIBS})

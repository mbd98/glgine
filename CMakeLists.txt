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
include_directories(musicShow/src thirdparty/irrKlang/include thirdparty/imgui colourroom/src upsideDownRoom/src)

aux_source_directory(thirdparty/imgui imgui)
add_library(ImGuiLib ${imgui})

aux_source_directory(musicShow/src SOURCE)
aux_source_directory(colourroom/src SOURCE)
aux_source_directory(upsideDownRoom/src SOURCE)
message(${SOURCE})
link_directories(thirdparty/irrKlang/bin/linux-gcc-64)
add_executable(project main.cpp ${SOURCE})
target_link_libraries(project ImGuiLib OpenGL::GL glew_s glfw glm fftw3 libIrrKlang.so ${CMAKE_DL_LIBS})
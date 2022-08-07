//
// Created by nikolas on 8/3/22.
//
#include "MusicNote.h"
#include "Renderer.h"
#include "Shader.h"

Renderer::Renderer() {
    main_shader = Shader::CreateShader("../musicShow/shaders/Vertex.shader","../musicShow/shaders/Fragment.shader");
    glUseProgram(main_shader);
}

Renderer::~Renderer() {

}

void Renderer::render() {

    for(auto& note : notes){
        note.setWidth(1/(float)notes.size());

        note.setUniforms(main_shader);
        note.draw();
    }
}

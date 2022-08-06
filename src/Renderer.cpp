//
// Created by nikolas on 8/3/22.
//
#include "MusicNote.h"
#include "Renderer.h"
#include "Shader.h"

Renderer::Renderer() {
    shader = Shader::CreateShader("../shaders/Vertex.shader","../shaders/Fragment.shader");

    glUniformMatrix4fv(glGetUniformLocation(shader, "u_View"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "u_Projection"), 1, GL_FALSE, &projection[0][0]);
}

Renderer::~Renderer() {

}

void Renderer::render() {

    glUseProgram(shader);
    for(auto& note : notes){
        note.setColor(random_color());
        note.setWidth(1/(float)notes.size());
//        note.addTransform(glm::scale(glm::mat4(1.0),{(1.f/(float)notes.size()),1,1}));
        note.addTransform(glm::translate(glm::mat4(1.0),{0.0,-0.5,0.0}));

        note.setUniforms(shader);
        note.draw();
    }
}

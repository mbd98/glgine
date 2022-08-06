//
// Created by nikolas on 8/3/22.
//

#include "MusicNote.h"

void MusicNote::draw() {
    glDrawElements(GL_TRIANGLES, (int)m_drawSize, GL_UNSIGNED_INT, nullptr);
}

MusicNote::MusicNote(float freq, float ampl) : m_Frequency(freq), m_Amplitude(ampl){
}
void MusicNote::createLayout(){

    glGenVertexArrays(1,&m_VAO);
    glBindVertexArray(m_VAO);

    std::vector<Vertex> vertices = { {{0.0f,0.0f,-1.0f}},
                                     {{1.0,0.0,-1.0f}},
                                     {{1.0,1.0,-1.0f}},
                                     {{0.0,1.0,-1.0f}}
    };
    std::vector<GLuint> indices = {
        0,1,2,
        0,2,3
    };
    createVertexBuffer(vertices);
    createIndexBuffer(indices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3* sizeof(float),(const void*)0);
}

void MusicNote::setColor(const glm::vec3 c) {
    m_Color = c;
}
float map(float a1, float a2, float b1, float b2, float val){
    float x = (val - a1) / (a2 - a1);

    float result = b1 + (b2 - b1) * x;
    return result;
}
void MusicNote::setUniforms(GLuint shader) {

    glm::mat4 model = glm::translate(glm::mat4(1.0f),{2*m_Frequency/ FREQUENCY_CUTOFF - 1,-1,0} );
    model = glm::scale(model,{m_Width,m_Amplitude * 0.05,1});

    if(m_Frequency < FREQUENCY_CUTOFF/3.f){
        float comp = map(0,FREQUENCY_CUTOFF/3.f,0,1.0,m_Frequency);
        m_Color = {0.0,comp,1.0 - comp};

    }
    else if( ((FREQUENCY_CUTOFF/3.f) < m_Frequency) && (m_Frequency < (2.f * FREQUENCY_CUTOFF/3.f))){
        float comp = map(FREQUENCY_CUTOFF/3.f,2.f * FREQUENCY_CUTOFF/3.f,0,1.0,m_Frequency);
        m_Color = {comp,1.0-comp,0.0};
    }
    else{
        float comp = map(2.f * FREQUENCY_CUTOFF/3.f,FREQUENCY_CUTOFF,0,1.0,m_Frequency);
        m_Color = {1.0,0,comp};
    }
    glUniform3f(glGetUniformLocation(shader, "u_Color"), m_Color.x, m_Color.y, m_Color.z);

//    model = glm::translate(model,{-4*m_Width,0,0});
//    for(glm::mat4 transform : m_Transforms){
//        model *= transform ;
//    }
//    m_Transforms.clear();
//    model = glm::translate(model,{-m_Width,0,0} );
    glUniformMatrix4fv(glGetUniformLocation(shader, "u_Model"), 1, GL_FALSE, &model[0][0]);
}

void MusicNote::createVertexBuffer(std::vector<Vertex> vertices) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
}

void MusicNote::createIndexBuffer(std::vector<GLuint> indices) {

    GLuint vbi;
    glGenBuffers(1, &vbi);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbi);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),&indices[0],GL_STATIC_DRAW);

}

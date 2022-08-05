//
// Created by nikolas on 8/3/22.
//

#ifndef COMP371_A1_MUSICNOTE_H
#define COMP371_A1_MUSICNOTE_H

#define FREQUENCY_CUTOFF 6000

#include "Mesh.h"

class MusicNote : public Mesh{
    float m_Width = 1.0;
    float m_Frequency;
    float m_Amplitude;
    glm::vec3 m_Color;

    inline static GLuint m_VAO;

    public:

    explicit MusicNote(float,float);

    static void createVertexBuffer(std::vector<Vertex> vertices) ;
    static void createIndexBuffer(std::vector<GLuint> vertices) ;
    static void createLayout() ;

    inline void setWidth(const float width) {m_Width = width;}
    inline GLuint getVAO() const {return m_VAO;}
    void setColor(const glm::vec3);
    void draw() override;
    void setUniforms(GLuint shader) override;
};


#endif //COMP371_A1_MUSICNOTE_H

#ifndef COMP371_A1_MESH_H
#define COMP371_A1_MESH_H

#include "Config.h"

class Mesh {

public:
    Mesh();
    virtual ~Mesh();
    void addTransform(const glm::mat4& );

protected:
    std::vector<glm::mat4> m_Transforms;
    const GLuint m_drawSize = 6;

    struct Vertex{
        glm::vec3 position;
    };

    virtual void setUniforms(GLuint shader) = 0;
    virtual void draw() = 0;
//    virtual void createVertexBuffer(std::vector<Vertex> vertices) = 0;
//    virtual void createIndexBuffer(std::vector<GLuint> vertices) = 0;
//    virtual void createLayout() = 0;
};


#endif //COMP371_A1_MESH_H

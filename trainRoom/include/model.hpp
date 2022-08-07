#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "renderable.hpp"
#include "complexrenderable.hpp"
#include "textures.hpp"

namespace Train {

    struct Material {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        GLfloat shine;

        Material();

        Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shine);

        Material(const Material &m);
    };

    class Mesh : public ComplexRenderable {
    private:
        IndexedRenderable *internal;
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

        ~Mesh() override;

        void render(GLuint shader) override;
    };

    class Model : public ComplexRenderable {
    private:
        std::vector<Mesh *> meshes;
    public:
        explicit Model(const std::string &name);

        ~Model() override;

        void render(GLuint shader) override;
    };
}
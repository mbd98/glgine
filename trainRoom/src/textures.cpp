
#include <string>
#include <stdexcept>
#include <stb_image.h>
#include <textures.hpp>
#include <iostream>

namespace Train {
    GLuint loadTexture(const char *path) {
        GLint width, height, channels;
        GLubyte *data;
        GLuint texture;
        GLenum type;

        std::cerr << "Loading texture " << path << std::endl;

        if ((data = stbi_load(path, &width, &height, &channels, 0)) == nullptr)
            throw std::runtime_error(std::string("Failed to load texture data for ") + std::string(path));

        switch (channels) {
            case 1:
                type = GL_RED;
                break;
            case 3:
                type = GL_RGB;
                break;
            case 4:
                type = GL_RGBA;
                break;
            default:
                type = 0;
        }
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }
}
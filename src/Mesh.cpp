//
// Created by nikolas on 8/3/22.
//

#include "Mesh.h"

void Mesh::addTransform(const glm::mat4 & t) {
    m_Transforms.emplace_back(t);
}

Mesh::Mesh() {
}

Mesh::~Mesh() {

}

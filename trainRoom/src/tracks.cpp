#include <tracks.hpp>
namespace Train {
    Tracks::Tracks() {
        segments[0] = new Reusable(new Model("track_straight"));
        segments[1] = new Reusable(new Model("track_sbend"));
        segments[2] = new Reusable(new Model("track_slight"));
        segments[3] = new Reusable(new Model("track_180"));

        for (auto &s: segments)
            s->setParent(this);

        //segments[0]->insertTransform(glm::mat4(1.0f));
        //segments[0]->insertTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 150.0f)));
        segments[3]->insertTransform(glm::mat4(1.0f));
        segments[3]->insertTransform(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    Tracks::~Tracks() {
        for (auto &s: segments)
            delete s;
    }

    void Tracks::render(GLuint shader) {
        for (auto &s: segments)
            s->render(shader);
    }
}
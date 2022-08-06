//
// Created by nikolas on 8/3/22.
//

#ifndef COMP371_PROJECT_RENDERER_H
#define COMP371_PROJECT_RENDERER_H
#include "Config.h"
#include "MusicNote.h"

class Renderer{
    std::vector<MusicNote> notes;
    GLuint shader;

    glm::mat4 view;
    glm::mat4 projection;

public:

    Renderer();
    ~Renderer();
    void render();
    inline void addNote(MusicNote& note){
        notes.push_back(note);
    }
    inline void clearNotes(){
        notes.clear();
    }
};

#endif //COMP371_PROJECT_RENDERER_H

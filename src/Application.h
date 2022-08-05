//
// Created by nikolas on 8/3/22.
//

#ifndef COMP371_PROJECT_APPLICATION_H
#define COMP371_PROJECT_APPLICATION_H
#include "AudioManager.h"
#include "Renderer.h"
#include "AudioFile.h"
#include <ctime>
#include "chrono"
class Application {

    std::chrono::steady_clock::time_point begin ;

    std::vector<float> frequencies;
    GLFWwindow* window;
    Renderer* renderer;
    AudioFile<double> audioFile;

    static inline int N = 1024;
    int song_index = 0;

    public:

    void initializeCore();
    void initializeAudioSampler(const std::string&);
    void playMusic(const std::string&);
    void updateAudio();
    void run();

};


#endif //COMP371_PROJECT_APPLICATION_H

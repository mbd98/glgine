//
// Created by nikolas on 8/3/22.
//

#ifndef COMP371_PROJECT_MUSICAPPLICATION_H
#define COMP371_PROJECT_MUSICAPPLICATION_H
#include "AudioManager.h"
#include "MenuManager.h"
#include "Renderer.h"
#include "AudioFile.h"
#include <ctime>
#include "chrono"

class MusicApplication {

    std::chrono::steady_clock::time_point begin,start ;

    std::vector<float> frequencies;
    GLFWwindow* window;
    Renderer* renderer;
    AudioFile<double> audioFile;

    SongInformation songInfo;

    irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

    static inline int fft_size = 1024;
    int song_index = 0;

    public:

    MusicApplication();
    void initializeCore();
    void initializeAudioSampler(const std::string&);
    void playMusic(const std::string&);
    void updateAudio();
    void run();

};


#endif //COMP371_PROJECT_MUSICAPPLICATION_H

//
// Created by nikolas on 8/3/22.
//

#include "MusicApplication.h"

void MusicApplication::initializeCore() {
    window = glfwCreateWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "emaj7b9#11", NULL, NULL);
    glfwMakeContextCurrent(window);

    MenuManager::initImGui(window);
    MenuManager::createFileBrowser();
    renderer = new Renderer();
}

void MusicApplication::run() {

    do {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MenuManager::MusicPlayerImGUIUpdate(songInfo);

        if(songInfo.has_started){
            begin = std::chrono::steady_clock::now();
            start = std::chrono::steady_clock::now();

            initializeAudioSampler(songInfo.name);
            playMusic(songInfo.name);

            songInfo.currently_playing = true;
            songInfo.duration = (int)audioFile.getLengthInSeconds();
            songInfo.has_started = false;
        }

        if(songInfo.currently_playing)
            updateAudio();

        renderer->render();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    MenuManager::endImGui();
}

void MusicApplication::updateAudio() {

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    songInfo.current_time = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count();

    if (delta >= 50) {

        double buf[fft_size];
        for (int i = 0; i < fft_size; i++) {
            buf[i] = audioFile.samples[0][i + song_index];
        }
        song_index += (int)((songInfo.sample_rate) / 15);

        AudioManager::setInputData(buf);

        double freqs[fft_size/2];
        double mags[fft_size/2];

        AudioManager::executeFFT(freqs, mags);

        std::vector<std::pair<float, float>> spectrum;
        spectrum.reserve(fft_size / 2);

        for (int i = 0; i < fft_size / 2; i++) {
            spectrum.emplace_back(std::make_pair(freqs[i], mags[i]));
        }
        renderer->clearNotes();
        for (const auto &note: spectrum) {
            if(note.first < FREQUENCY_CUTOFF) {
                auto *m = new MusicNote(note.first, note.second);
                renderer->addNote(*m);
            }
        }

        begin = std::chrono::steady_clock::now();
    }
}

void MusicApplication::initializeAudioSampler(const std::string& path) {

    audioFile.load(path);
    audioFile.printSummary();
    songInfo.sample_rate = audioFile.getSampleRate();
    AudioManager::init(songInfo.sample_rate,fft_size);
    MusicNote::createLayout();
}

void MusicApplication::playMusic(const std::string& path) {

    irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();
    SoundEngine->play2D( path.c_str(), false);

}

MusicApplication::MusicApplication() {

}




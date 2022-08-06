//
// Created by nikolas on 8/3/22.
//

#include <fftw3.h>
#include "Application.h"
#include "time.h"

void Application::initializeCore() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }

    //glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open a window and create its OpenGL context
        window = glfwCreateWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "Jam", NULL, NULL);
        if (window == nullptr) {
            fprintf(stderr,
                    "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
            glfwTerminate();
        }
        glfwMakeContextCurrent(window); // Initialize GLEW

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }
    std::cout << "Running openGL version : " << glGetString(GL_VERSION) << std::endl;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    MenuManager::initImGui(window);
    MenuManager::createFileBrowser();
    renderer = new Renderer();
}

void Application::run() {


//    music_file = "../audio/ichika.wav";
    do {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MenuManager::imGuiUpdate(songInfo);


        if(songInfo.has_started){
            begin = std::chrono::steady_clock::now();
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

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    MenuManager::endImGui();
}
//
void Application::updateAudio() {

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

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

void Application::initializeAudioSampler(const std::string& path) {

    audioFile.load(path);
    audioFile.printSummary();
    songInfo.sample_rate = audioFile.getSampleRate();
    AudioManager::init(songInfo.sample_rate,fft_size);
    MusicNote::createLayout();
}

void Application::playMusic(const std::string& path) {

    irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();
    SoundEngine->play2D( path.c_str(), true);

}
//



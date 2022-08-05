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

    renderer = new Renderer();

}

void Application::run() {

    begin = std::chrono::steady_clock::now();
    do {

        updateAudio();
        glClear(GL_COLOR_BUFFER_BIT);
        renderer->render();
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

}

void Application::updateAudio() {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count();


    if (delta >= 50) {

        begin = std::chrono::steady_clock::now();

        double buf[N];
        for (int i = 0; i < N; i++) {
            buf[i] = audioFile.samples[0][i + song_index];
        }
        song_index += (int)(audioFile.getSampleRate()) / 17;

        AudioManager::setInputData(buf);

        double freqs[N];
        double mags[N];

        AudioManager::executeFFT(freqs, mags);

        std::vector<std::pair<float, float>> spectrum;
        spectrum.reserve(N / 2);

        for (int i = 0; i < N / 2; i++) {
            spectrum.emplace_back(std::make_pair(freqs[i], mags[i]));
        }
        renderer->clearNotes();
        for (const auto &note: spectrum) {
            auto *m = new MusicNote(note.first, note.second);
            renderer->addNote(*m);;
        }
    }
}

void Application::initializeAudio() {

    audioFile.load("../song.wav");
    audioFile.printSummary();

    irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();
    SoundEngine->play2D("../ichika_good.mp3", true);

    AudioManager::setBufferSize(N);
    AudioManager::init(audioFile.getSampleRate());

    MusicNote::createLayout();
}

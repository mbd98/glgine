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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::StyleColorsDark();

    ImGui::GetIO().FontGlobalScale *= 2;
    ImGui::GetStyle().ScaleAllSizes(5);

renderer = new Renderer();

}

void Application::run() {
    ImGui::FileBrowser fileDialog;
    fileDialog.SetTitle("title");
    fileDialog.SetTypeFilters({ ".mp3", ".wav" });

    begin = std::chrono::steady_clock::now();
    initializeAudioSampler("../audio/more_ichika.wav");
    playMusic("../audio/more_ichika.mp3");
    do {

//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        ImGui::SetNextWindowPos({0,0});
//        if(ImGui::Begin("dummy window"))
//        {
//            // open file dialog when user clicks this button
//            if(ImGui::Button("open file dialog"))
//                fileDialog.Open();
//        }
//        ImGui::End();//
//        fileDialog.Display();
//        if(fileDialog.HasSelected())
//        {
//            std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
//            fflush(stdout);
//            fileDialog.ClearSelected();
//            fileDialog.Close();
//        }
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        updateAudio();
        glClear(GL_COLOR_BUFFER_BIT);
        renderer->render();
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
        song_index += (int)(audioFile.getSampleRate()) / 17.5;

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
            if(note.first < FREQUENCY_CUTOFF) {
                auto *m = new MusicNote(note.first, note.second);
                renderer->addNote(*m);
            }
        }
    }
}

void Application::initializeAudioSampler(const std::string& path) {

    audioFile.load(path);
    audioFile.printSummary();
    AudioManager::init(audioFile.getSampleRate(),N);
    MusicNote::createLayout();
}

void Application::playMusic(const std::string& path) {

    irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();
    SoundEngine->play2D( path.c_str(), true);

}




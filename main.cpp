#define STB_IMAGE_IMPLEMENTATION
#include "MusicApplication.h"
#include "colourroom_nomain.h"
#include "upsideDownRoom.h"
#include "train.hpp"

GLFWwindow* main_window = nullptr;


void initMainMenu(){
    main_window = glfwCreateWindow(800, 1000, "emaj7b9#11", NULL, NULL);
    glfwMakeContextCurrent(main_window);
    glewInit();

    MenuManager::initImGui(main_window);
}
// List of functions that start each room
void startMusicApplication(){

    MusicApplication app;
    MenuManager::endImGui();
    app.run();
    MenuManager::initImGui(main_window);
}
void startUpsideDownRoomApplication(){
    upsideDownRoom::upsideDownRoomMain();
}
void startColorRoomApplication(){
    colorRoom::colourroom();
}
void startTrainRoomApplication(){
    Train::teleportingTrain();
}
int main() {

    // Create basic GLFW window for menu rendering
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    main_window = glfwCreateWindow(800, 1000, "emaj7b9#11", NULL, NULL);

    glfwMakeContextCurrent(main_window);
    glewInit();

    // Init ImGUI using the created window
    MenuManager::initImGui(main_window);

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the main menu using ImGUI
        MenuManager::RenderMainMenu(startMusicApplication,
                                    startColorRoomApplication,
                                    startUpsideDownRoomApplication,
                                    startTrainRoomApplication);

        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }
    while (glfwGetKey(main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(main_window) == 0);


    return 0;
}

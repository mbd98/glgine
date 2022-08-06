#include "MusicApplication.h"
#include "colourroom_nomain.h"

GLFWwindow* main_window = nullptr;


void initMainMenu(){
    main_window = glfwCreateWindow(800, 1000, "emaj7b9#11", NULL, NULL);
    glfwMakeContextCurrent(main_window);
    glewInit();

    MenuManager::initImGui(main_window);
}
// List of functions that start each room
void startMusicApplication(){

    glfwDestroyWindow(main_window);
    MenuManager::endImGui();

    MusicApplication app;
    app.initializeCore();
    app.run();

    initMainMenu();
}

void startColorRoomApplication(){
    glfwDestroyWindow(main_window);
    MenuManager::endImGui();

    colorRoom::createWindow();
    colorRoom::colourroom();

    initMainMenu();
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
        MenuManager::RenderMainMenu(startMusicApplication,startColorRoomApplication);

        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }
    while (glfwGetKey(main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(main_window) == 0);


    return 0;
}

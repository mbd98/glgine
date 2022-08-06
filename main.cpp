#include "MusicApplication.h"

GLFWwindow* window = nullptr;

// List of functions that start each room
void startMusicApplication(){
    MusicApplication app(window);
    app.initializeCore();
    app.run();
}


int main() {

    // Create basic GLFW window for menu rendering
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     window = glfwCreateWindow(800, 1000, "emaj7b9#11", NULL, NULL);

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }

    // Init ImGUI using the created window
    MenuManager::initImGui(window);

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the main menu using ImGUI
        MenuManager::RenderMainMenu(startMusicApplication);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);


    return 0;
}

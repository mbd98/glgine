#define STB_IMAGE_IMPLEMENTATION
#include "MusicApplication.h"
#include "colourroom_nomain.h"
#include "upsideDownRoom.h"
#include "train.hpp"

GLFWwindow* main_window = nullptr;

int resolution_x = 0;
int resolution_y = 0;


int menu_width = 800;
int menu_height = 500;

void initMainMenu(){
    main_window = glfwCreateWindow(menu_width, menu_height , "COMP 371 Project", NULL, NULL);
    glfwMakeContextCurrent(main_window);

    resolution_x = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    resolution_y = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

    glfwSetWindowPos(main_window,resolution_x/2 - menu_width/2,resolution_y/2 - menu_height/2);
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
    glfwSetWindowPos(main_window,resolution_x/2 - menu_width/2,resolution_y/2 - menu_height/2);
}
void startColorRoomApplication(){
    colorRoom::colourroom();
    glfwSetWindowPos(main_window,resolution_x/2 - menu_width/2,resolution_y/2 - menu_height/2);
}
void startTrainRoomApplication(){
    Train::teleportingTrain();
    glfwSetWindowPos(main_window,resolution_x/2 - menu_width/2,resolution_y/2 - menu_height/2);

}
int main() {

    // Create basic GLFW window for menu rendering
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    initMainMenu();
//    main_window = glfwCreateWindow(800, 1000, "emaj7b9#11", NULL, NULL);
//
//
//    glfwMakeContextCurrent(main_window);
//    glewInit();
//
//    // Init ImGUI using the created window
//    MenuManager::initImGui(main_window);

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

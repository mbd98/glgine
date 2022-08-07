//
// Created by nikolas on 8/6/22.
//

#ifndef COMP371_PROJECT_MENUMANAGER_H
#define COMP371_PROJECT_MENUMANAGER_H

#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imfilebrowser.h"

struct SongInformation{
    std::string name ;
    int current_time = 0;
    int duration = 0;
    int sample_rate = 0;
    bool has_started = false;
    bool currently_playing = false;
};

class MenuManager {
    inline static ImGui::FileBrowser* fileDialog = nullptr;

public:


    static void initImGui(GLFWwindow* window){

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplGlfw_InitForOpenGL(window,true);
        ImGui_ImplOpenGL3_Init("#version 460");

        ImGui::StyleColorsDark();

        ImGui::GetIO().FontGlobalScale *= 2;
        ImGui::GetStyle().ScaleAllSizes(5);

    }

    static void RenderMainMenu(void (*musicApplicationCallback)(),void (*colorRoomCallback)(),
                               void (*upsideDownRoomCallback)(),void (*startTrainRoomCallback)()){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos({50,50});
        if(ImGui::Begin("Main menu")) {

            if (ImGui::Button("Click me to start the teleporting train")) {

                ImGui::End();
                ImGui::Render();
                startTrainRoomCallback();
                return;
            }
            else if (ImGui::Button("Click me to start the music player")) {

                ImGui::End();
                ImGui::Render();
                musicApplicationCallback();
                return;
            }
            else if (ImGui::Button("Click me to start the sphere color room")) {

                ImGui::End();
                ImGui::Render();
                colorRoomCallback();
                return;
            }
            else if (ImGui::Button("Click me to start the lights room")) {

                ImGui::End();
                ImGui::Render();
                upsideDownRoomCallback();
                return;
            }

        }

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
    static void endImGui(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    static void createFileBrowser(){
        fileDialog = new ImGui::FileBrowser();
        fileDialog->SetWindowSize(1000,600);
        fileDialog->SetTitle("Select song");

        fileDialog->SetTypeFilters({ ".wav" });
    }
    static void MusicPlayerImGUIUpdate(SongInformation& songInfo){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos({0,0});
        if(ImGui::Begin("Select music",NULL,ImGuiWindowFlags_NoMove))
        {
            if(ImGui::Button("Browse files"))
                fileDialog->Open();
        }
        ImGui::End();//

        ImGui::SetNextWindowPos({600,0});
        ImGui::SetNextWindowSize({1400,150});
        std::string infoTitle = "Now playing : " + songInfo.name;

        if(ImGui::Begin(infoTitle.c_str()))
        {
            if(songInfo.currently_playing) {
                 ImGui::Text("Sampling Rate : %i Hz - %i/%i (s)", songInfo.sample_rate,
                             songInfo.current_time,songInfo.duration);
            }
        }
        ImGui::End();

        fileDialog->Display();
        if(fileDialog->HasSelected())
        {
            songInfo.name = fileDialog->GetSelected().string();
            songInfo.has_started = true;
            fileDialog->ClearSelected();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};


#endif //COMP371_PROJECT_MENUMANAGER_H

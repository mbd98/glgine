//
// Created by nikolas on 8/6/22.
//

#ifndef COMP371_PROJECT_MENUMANAGER_H
#define COMP371_PROJECT_MENUMANAGER_H

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
        ImGui_ImplOpenGL3_Init("#version 330");

        ImGui::StyleColorsDark();

        ImGui::GetIO().FontGlobalScale *= 2;
        ImGui::GetStyle().ScaleAllSizes(5);

    }
    static void endImGui(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    static void createFileBrowser(){
        fileDialog = new ImGui::FileBrowser();
        fileDialog->SetTitle("fuck you");

        fileDialog->SetTypeFilters({ ".mp3", ".wav" });
    }
    static void imGuiUpdate(SongInformation& songInfo){
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
        //ImGui::SetNextWindowSize({1000,600});
        std::string infoTitle = "Now playing : " + songInfo.name;
        if(ImGui::Begin(infoTitle.c_str()))
        {
            if(songInfo.currently_playing) {
//                std::string text = std::to_string(songInfo.sample_rate) + " Hz ";
                ImGui::Text("%i Hz", songInfo.sample_rate);
            }
        }
        ImGui::End();

        fileDialog->Display();
        if(fileDialog->HasSelected())
        {
//            std::cout << "Selected filename" << fileDialog->GetSelected().string() << std::endl;
            fflush(stdout);
            songInfo.name = fileDialog->GetSelected().string();
            songInfo.has_started = true;
            fileDialog->ClearSelected();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};


#endif //COMP371_PROJECT_MENUMANAGER_H

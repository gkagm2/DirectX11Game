#include "pch.h"
#include "MainMenuGUI.h"

MainMenuGUI::MainMenuGUI() :
    bPlay(true),
    bPause(false),
    bStop(false)
{
    SetName(STR_GUI_MainMenu);
}

MainMenuGUI::~MainMenuGUI()
{
}

void MainMenuGUI::Update()
{
    if (ImGui::BeginMainMenuBar())
    {
        // Scene Menu
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Save Scene", "CTLR+S", false ,false)) {

            }
            if (ImGui::MenuItem("Load Scene", "CTRL+L", false, false)) {

            }
            ImGui::EndMenu();
        }

        // SceneMode
        if (ImGui::BeginMenu("Mode")) {
            ShowSceneMode();
            ImGui::EndMenu();
        }

        // File Menu
        if(ImGui::BeginMenu("File")) {


            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void MainMenuGUI::ShowExampleMenuFile()
{
    ImGui::MenuItem("(demo menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}

void MainMenuGUI::ShowSceneMode()
{
    if (ImGui::MenuItem("Play", "CTLR+P", false, bPlay)) {
        bPause = true;
        bStop = true;
        bPlay = false;
    }
    if (ImGui::MenuItem("Pause", "CTRL+P", false, bPause)) {
        bPause = false;
        bPlay = true;
        bStop = true;
    }
    if (ImGui::MenuItem("Stop", "CTRL+O", false, bStop)) {
        bStop = false;
        bPlay = true;
        bPause = false;
    }
}
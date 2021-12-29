#include "pch.h"
#include "MainMenuGUI.h"
#include <Engine\CCore.h>
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>
#include <Engine\CObjectManager.h>

#include "CSceneSaveLoad.h"
#include "InspectorGUI.h"
#include "CImGuiManager.h"

#include "HierarchyGUI.h"
#include "ResourceViewGUI.h"
#include "SceneViewGUI.h"
#include "DebugGUI.h"

#include "TileMapEditorGUI.h"
#include "CollisionEditorGUI.h"
#include "Animator2DEditorGUI.h"
#include "ToolCameraGUI.h"

// Captain Forever Game
#include "ModuleCreatorGUI_ca.h"

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
    if (E_SceneMode::Stop == CSceneManager::GetInstance()->GetSceneMode()) {
        if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::S))
            SaveScene();
        if (InputKeyHold(E_Key::LCtrl) && InputKeyPress(E_Key::L))
            LoadScene();
    }

    if (ImGui::BeginMainMenuBar())
    {
        // Scene Menu
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Create New Scene(no save)")) {
                CreateNewScene();
            }
            if (ImGui::MenuItem("Save Scene", "CTLR+S", false, true)) {
                
                SaveScene();
            }
            if (ImGui::MenuItem("Load Scene", "CTRL+L", false, true)) {

                LoadScene();
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
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Object")) {
            if (ImGui::MenuItem("Create GameObject")) {
                CreateEmptyGameObject();
            }
            if (ImGui::MenuItem("Create 2DCamera")) {
                CreateCamera2DGameObject();
            }
            if (ImGui::MenuItem("Create 2DRect")) {
                Create2DRectGameObject();
            }
            if (ImGui::MenuItem("Create Material")) {
                CreateEmptyMaterial();
            }
            if (ImGui::MenuItem("Create ParticleSystem")) {
                CreateParticleSystemGameObject();
            }
            if (ImGui::MenuItem("Create Light2D")) {
                CreateLight2D();
            }
            if (ImGui::MenuItem("Create Cursor")) {
                CreateCursorUI();
            }
            if (ImGui::BeginMenu("Create UI")) {
                if (ImGui::MenuItem("Button UI")) {
                    CreateButtonUI();
                }
                if (ImGui::MenuItem("Image UI")) {
                    CreateImageUI();
                }
                if (ImGui::MenuItem("Text UI")) {
                    CreateTextUI();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("CF Game")) {
            if (ImGui::MenuItem("Modeul Creator Tool")) {
                OpenModuleCreatorToolWindows();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Editor")) {
            if (ImGui::MenuItem("Tile Map Editor")) {
                OpenTileMapEditor();
            }
            if (ImGui::MenuItem("Animator2D Editor")) {
                OpenAnimator2DEditor();
            }
            if (ImGui::MenuItem("Collision Editor")) {
                OpenCollisionEditor();
            }
            if (ImGui::BeginMenu("Engine Tools")) {
                if (ImGui::MenuItem("Tool Camera")) {
                    OpenToolCameraUI();
                }
                if (ImGui::MenuItem("Hierarchy")) {
                    OpenHierarchyUI();
                }
                if (ImGui::MenuItem("Resource")) {
                    OpenResourceViewUI();
                }
                if (ImGui::MenuItem("Scene")) {
                    OpenSceneViewUI();
                }
                if (ImGui::MenuItem("Debug")) {
                    OpenDebugUI();
                }
                ImGui::EndMenu();
            }
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

void MainMenuGUI::SaveScene()
{
    OPENFILENAME ofn;
    TCHAR strMaxPath[MAX_PATH] = _T("");
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
    ofn.lpstrFilter = _T(".scene");//_T("모든파일(*.*)\0*.*\0*");
    ofn.lpstrFile = strMaxPath;
    ofn.nMaxFile = MAX_PATH;

    if (0 != GetSaveFileName(&ofn)) {
        // Save
        tstring path = ofn.lpstrFile;
        CSceneSaveLoad::SaveScene(CSceneManager::GetInstance()->GetCurScene(), path, false);

        TCHAR str[255] = _T("Scene File Save");
        CObject::ChangeStateEvn();
        MessageBox(CCore::GetInstance()->GetWndHandle(), str, _T("Save"), MB_OK);
    }
}

void MainMenuGUI::LoadScene()
{
    OPENFILENAME ofn;
    TCHAR strMaxPath[MAX_PATH] = _T("");
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
    ofn.lpstrFilter = _T(".anim");// _T("모든파일(*.*)\0*.*\0*");
    ofn.lpstrFile = strMaxPath;
    ofn.nMaxFile = MAX_PATH;


    if (0 != GetOpenFileName(&ofn)) {
        // Load
        tstring path = ofn.lpstrFile;
        CScene* pCurScene = CSceneSaveLoad::LoadScene(path, false);
        CSceneManager::GetInstance()->ChangeSceneEvt(pCurScene);
        InspectorGUI* pInspectorGUI = (InspectorGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
        pInspectorGUI->SetInspectorUIMode(E_InspectorUIMode::None);
        TCHAR str[255] = _T("Scene File Load");
        MessageBox(CCore::GetInstance()->GetWndHandle(), str, _T("Load"), MB_OK);
    }
}

void MainMenuGUI::ShowSceneMode()
{
    if (ImGui::MenuItem("Play", "CTLR+P", false, bPlay)) {
        _SetSceneModeMenu(E_SceneMode::Play);
        CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Play);
        CSceneSaveLoad::SaveScene(CSceneManager::GetInstance()->GetCurScene(), STR_FILE_PATH_TempScene);
    }
    if (ImGui::MenuItem("Pause", "CTRL+P", false, bPause)) {
        _SetSceneModeMenu(E_SceneMode::Pause);
        CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Pause);
    }
    if (ImGui::MenuItem("Stop", "CTRL+O", false, bStop)) {
        _SetSceneModeMenu(E_SceneMode::Stop);
        CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Stop);
        CScene* pScene = CSceneSaveLoad::LoadScene(STR_FILE_PATH_TempScene);
        CSceneManager::GetInstance()->ChangeSceneEvt(pScene);
    }
}

void MainMenuGUI::_SetSceneModeMenu(E_SceneMode _eMode)
{
    switch (_eMode) {
    case E_SceneMode::Play:
        bPause = true;
        bStop = true;
        bPlay = false;
        break;
    case E_SceneMode::Pause:
        bPause = false;
        bPlay = true;
        bStop = true;
        break;
    case E_SceneMode::Stop:
        bStop = false;
        bPlay = true;
        bPause = false;
        break;
    }
}

void MainMenuGUI::CreateEmptyMaterial()
{
    CObjectManager::GetInstance()->CreateEmptyMaterial();
}

CGameObject* MainMenuGUI::CreateEmptyGameObject()
{
    return CObjectManager::GetInstance()->CreateEmptyGameObject();
}

void MainMenuGUI::CreateCamera2DGameObject()
{
    CObjectManager::GetInstance()->CreateCamera2DGameObject();
}

void MainMenuGUI::CreateParticleSystemGameObject()
{
    CObjectManager::GetInstance()->CreateParticleSystemGameObject();
}

void MainMenuGUI::CreateLight2D()
{
    CObjectManager::GetInstance()->CreateLight2D();
}

void MainMenuGUI::CreateNewScene()
{
    CSceneManager::GetInstance()->CreateNewScene();
}

void MainMenuGUI::Create2DRectGameObject()
{
    CObjectManager::GetInstance()->Create2DRectGameObject();
}

void MainMenuGUI::CreateTextUI()
{   
    CObjectManager::GetInstance()->CreateTextUI();
}

void MainMenuGUI::CreateImageUI()
{
    CObjectManager::GetInstance()->CreateImageUI();
}

void MainMenuGUI::CreateButtonUI()
{
    CObjectManager::GetInstance()->CreateButtonUI();
}

#include <Script\CCursorScript.h>
void MainMenuGUI::CreateCursorUI()
{
    CGameObject* pCursor = CObjectManager::GetInstance()->CreateImageUI();
    Vector3 vPos = pCursor->RectTransform()->GetLocalPosition();
    vPos.y += 9999.f; // 화면 밖으로 숨김
    pCursor->RectTransform()->SetLocalPosition(vPos);
    pCursor->SetName(STR_OBJ_NAME_Cursor);
    pCursor->AddComponent<CCursorScript>();
}

void MainMenuGUI::OpenToolCameraUI()
{
    ToolCameraGUI* pGUI = dynamic_cast<ToolCameraGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ToolCamera));
    if (!pGUI) {
        assert(nullptr && _T("Tool Camera GUI를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenHierarchyUI()
{
    HierarchyGUI* pGUI = dynamic_cast<HierarchyGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Hierarchy));
    if (!pGUI) {
        assert(nullptr && _T("Hierarchy GUI를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenInspectorUI()
{
    InspectorGUI* pGUI = dynamic_cast<InspectorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector));
    if (!pGUI) {
        assert(nullptr && _T("Inspector GUI를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenResourceViewUI()
{
    ResourceViewGUI* pGUI = dynamic_cast<ResourceViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ResourceView));
    if (!pGUI) {
        assert(nullptr && _T("Resource View GUI를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}


void MainMenuGUI::OpenSceneViewUI()
{
    SceneViewGUI* pGUI = dynamic_cast<SceneViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_SceneView));
    if (!pGUI) {
        assert(nullptr && _T("Scene View GUI를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenDebugUI()
{
    DebugGUI* pGUI = dynamic_cast<DebugGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Debug));
    if (!pGUI) {
        assert(nullptr && _T("Debug GUI를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenModuleCreatorToolWindows()
{
    ModuleCreatorGUI_ca* pGUI = dynamic_cast<ModuleCreatorGUI_ca*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ModuleCreator));
    if (!pGUI) {
        assert(nullptr && _T("Module Creator를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenTileMapEditor()
{
    TileMapEditorGUI* pGUI = dynamic_cast<TileMapEditorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_TileMapEditor));
    if (!pGUI) {
        assert(nullptr && _T("Tile Map Editor를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenCollisionEditor()
{
    CollisionEditorGUI* pGUI = dynamic_cast<CollisionEditorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_CollisionEditor));
    if (!pGUI) {
        assert(nullptr && _T("Collsion Editor를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}

void MainMenuGUI::OpenAnimator2DEditor()
{
    Animator2DEditorGUI* pGUI = dynamic_cast<Animator2DEditorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Animator2DEditor));
    if (!pGUI) {
        assert(nullptr && _T("Animator2D Editor를 열 수 없다."));
        return;
    }
    pGUI->SetActive(true);
}
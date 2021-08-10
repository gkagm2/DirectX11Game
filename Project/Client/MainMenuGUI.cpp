#include "pch.h"
#include "MainMenuGUI.h"
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>
#include <Engine\CResourceManager.h>
#include <Engine\CMaterial.h>

#include <Engine\CRenderManager.h>
#include <Engine\CCamera.h>
#include <Engine\CTransform.h>
#include "CSceneSaveLoad.h"

UINT g_iMtrlID = 0;
UINT g_iEmptyGameObjectID = 0;

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
            if (ImGui::MenuItem("Create Material")) {
                CreateEmptyMaterial();
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

void MainMenuGUI::ShowSceneMode()
{
    if (ImGui::MenuItem("Play", "CTLR+P", false, bPlay)) {
        bPause = true;
        bStop = true;
        bPlay = false;
        CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Play);
        CSceneSaveLoad::SaveScene(CSceneManager::GetInstance()->GetCurScene(), STR_FILE_PATH_TempScene);
    }
    if (ImGui::MenuItem("Pause", "CTRL+P", false, bPause)) {
        bPause = false;
        bPlay = true;
        bStop = true;
        CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Pause);
    }
    if (ImGui::MenuItem("Stop", "CTRL+O", false, bStop)) {
        bStop = false;
        bPlay = true;
        bPause = false;
        CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Stop);
        CSceneSaveLoad::LoadScene(STR_FILE_PATH_TempScene);
    }
}

void MainMenuGUI::CreateEmptyMaterial()
{
    constexpr int iBuffSize = 255;
    TCHAR szBuff[iBuffSize]= _T("");

    // 고유 이름값 생성
    while (true) {
        _stprintf_s(szBuff, iBuffSize, _T("Material %d.mtrl"), g_iMtrlID++);
        CMaterial* pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(szBuff).Get();
        if (nullptr == pMtrl)
            break;
    }

    tstring strKey = szBuff;
    tstring strRelativePath = STR_FILE_PATH_Material;
    strRelativePath = strRelativePath + strKey;

    CMaterial* pNewMtrl = new CMaterial(true); // Create Default Material 
    pNewMtrl->SetKey(strKey);
    pNewMtrl->SetRelativePath(strRelativePath);

    CResourceManager::GetInstance()->AddRes<CMaterial>(strKey, pNewMtrl);
}

void MainMenuGUI::CreateEmptyGameObject()
{
    constexpr int iBuffSize = 255;
    TCHAR szBuffer[iBuffSize] = _T("");

    // 고유 이름값 생성
    while (true) {
        _stprintf_s(szBuffer, iBuffSize, _T("GameObject%d"), g_iEmptyGameObjectID++);
        CGameObject* pObj = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(szBuffer);
        if (nullptr == pObj)
            break;
    }

    // 새 게임 오브젝트 생성
    CGameObject* pNewGameObject = new CGameObject;
    pNewGameObject->SetName(szBuffer);
    pNewGameObject->AddComponent<CTransform>();

    // Tool Camera가 바라보고 있는 위치에 생성
    CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
    Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
    pNewGameObject->Transform()->SetLocalPosition(vWorldPos);
    CObject::CreateGameObjectEvn(pNewGameObject, E_Layer::Default);
}
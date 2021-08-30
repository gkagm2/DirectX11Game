#include "pch.h"
#include "MainMenuGUI.h"
#include <Engine\CCore.h>
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>
#include <Engine\CResourceManager.h>
#include <Engine\CMaterial.h>

#include <Engine\CRenderManager.h>
#include <Engine\CCamera.h>
#include <Engine\CTransform.h>

#include <Engine\CCanvasRenderer.h>
#include <Engine\CRectTransform.h>
#include <Engine\CTextUI.h>
#include <Engine\CButtonUI.h>
#include <Engine\CImageUI.h>

#include "CSceneSaveLoad.h"
#include "InspectorGUI.h"
#include "CImGuiManager.h"

#include "TileMapEditorGUI.h"

// Captain Forever Game
#include "ModuleCreatorGUI_ca.h"


UINT g_iMtrlID = 0;
UINT g_iEmptyGameObjectID = 0;
UINT g_iEmpty2DCameraGameObjectID = 0;
UINT g_iEmptyRect2DGameObjectID = 0;


UINT g_iTextUIGameObjectID = 0;
UINT g_iButtonUIGameObjectID = 0;
UINT g_iImageUIGameObjectID = 0;

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
            if (ImGui::MenuItem("Save Scene", "CTLR+S", false , true)) {
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
                Create2DRectGameObjet();
            }
            if (ImGui::MenuItem("Create Material")) {
                CreateEmptyMaterial();
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
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

tstring MainMenuGUI::_CreateObjectName(const tstring& _strObjDefaultName, UINT& id)
{
    constexpr int iBuffSize = 255;
    TCHAR szBuffer[iBuffSize] = _T("");

    // 고유 이름값 생성
    while (true) {
        _stprintf_s(szBuffer, iBuffSize, _T("%s%d"), _strObjDefaultName.c_str() , id++);
        CGameObject* pObj = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(szBuffer);
        if (nullptr == pObj)
            break;
    }
    return szBuffer;
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
    wchar_t strMaxPath[MAX_PATH] = L"";
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
    ofn.lpstrFilter = L"모든파일(*.*)\0*.*\0*";
    ofn.lpstrFile = strMaxPath;
    ofn.nMaxFile = MAX_PATH;

    if (0 != GetSaveFileName(&ofn)) {
        // Save
        tstring path = ofn.lpstrFile;
        CSceneSaveLoad::SaveScene(CSceneManager::GetInstance()->GetCurScene(), path, false);

        wchar_t str[255] = L"Tile File Save";
        CObject::ChangeStateEvn();
        MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Save", MB_OK);
    }
}

void MainMenuGUI::LoadScene()
{
    OPENFILENAME ofn;
    wchar_t strMaxPath[MAX_PATH] = L"";
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = CCore::GetInstance()->GetWndHandle();
    ofn.lpstrFilter = L"모든파일(*.*)\0*.*\0*";
    ofn.lpstrFile = strMaxPath;
    ofn.nMaxFile = MAX_PATH;


    if (0 != GetOpenFileName(&ofn)) {
        // Load
        tstring path = ofn.lpstrFile;
        CScene* pCurScene = CSceneSaveLoad::LoadScene(path, false);
        CSceneManager::GetInstance()->ChangeSceneEvt(pCurScene);
        InspectorGUI* pInspectorGUI = (InspectorGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
        pInspectorGUI->SetInspectorUIMode(E_InspectorUIMode::None);
        wchar_t str[255] = L"Tile File Load";
        MessageBox(CCore::GetInstance()->GetWndHandle(), str, L"Load", MB_OK);
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

    tstring strRelativePath = STR_FILE_PATH_Material;

    // 고유 이름값 생성
    while (true) {
        _stprintf_s(szBuff, iBuffSize, _T("%sMaterial %d.mtrl"), strRelativePath.c_str(), g_iMtrlID++);
        CMaterial* pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(szBuff).Get();
        if (nullptr == pMtrl)
            break;
    }

    tstring strKey = szBuff;
    strRelativePath = szBuff;
    CMaterial* pNewMtrl = new CMaterial(false); // Create Material 
    pNewMtrl->SetKey(strKey);
    pNewMtrl->SetRelativePath(strRelativePath);

    CResourceManager::GetInstance()->AddRes<CMaterial>(strKey, pNewMtrl);

    if (!pNewMtrl->IsDefaultMaterial() && E_SceneMode::Stop == CSceneManager::GetInstance()->GetSceneMode())
        pNewMtrl->Save(strRelativePath);
}

CGameObject* MainMenuGUI::CreateEmptyGameObject()
{
    tstring strObjName =_CreateObjectName(_T("GameObject"), g_iEmptyGameObjectID);

    // 새 게임 오브젝트 생성
    CGameObject* pNewGameObject = new CGameObject;
    pNewGameObject->SetName(strObjName);
    pNewGameObject->AddComponent<CTransform>();

    // Tool Camera가 바라보고 있는 위치에 생성
    CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
    
    Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
    pNewGameObject->Transform()->SetLocalPosition(vWorldPos);
    CObject::CreateGameObjectEvn(pNewGameObject, 0);

    return pNewGameObject;
}

void MainMenuGUI::CreateCamera2DGameObject()
{
    tstring strObjName = _CreateObjectName(_T("2D Camera"), g_iEmpty2DCameraGameObjectID);

    // 새 게임 오브젝트 생성
    CGameObject* pNewGameObject = new CGameObject;
    pNewGameObject->SetName(strObjName);
    pNewGameObject->AddComponent<CTransform>();
    pNewGameObject->AddComponent<CCamera>();
    pNewGameObject->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
    pNewGameObject->Camera()->SetLayerCheckAll();

    // Tool Camera가 바라보고 있는 위치에 생성
    CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
    Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
    pNewGameObject->Transform()->SetLocalPosition(vWorldPos);
    CObject::CreateGameObjectEvn(pNewGameObject, 0);
}

void MainMenuGUI::Create2DRectGameObjet()
{
    tstring strObjName = _CreateObjectName(_T("Rect GameObject"), g_iEmptyRect2DGameObjectID);

    // 새 게임 오브젝트 생성
    CGameObject* pNewGameObject = new CGameObject;
    pNewGameObject->SetName(strObjName);
    pNewGameObject->AddComponent<CTransform>();
    pNewGameObject->AddComponent<CMeshRenderer>();
    CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlendMtrl);


    // Tool Camera가 바라보고 있는 위치에 생성
    CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
    Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
    pNewGameObject->Transform()->SetLocalPosition(vWorldPos);
    CObject::CreateGameObjectEvn(pNewGameObject, 0);
}

CGameObject* MainMenuGUI::_CreateUIGameObject()
{
    CGameObject* pUIGameObject = new CGameObject;
    pUIGameObject->AddComponent<CRectTransform>();
    pUIGameObject->AddComponent<CCanvasRenderer>();
    return pUIGameObject;
}

CGameObject* MainMenuGUI::_CreateDefaultUICamera()
{
    tstring strObjName = STR_OBJ_NAME_UICamera;

    // 새 게임 오브젝트 생성
    CGameObject* pUICameraObj = new CGameObject;
    pUICameraObj->SetName(strObjName);
    pUICameraObj->AddComponent<CTransform>();
    pUICameraObj->AddComponent<CCamera>();
    pUICameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
    pUICameraObj->Camera()->SetLayerCheckAllUnActive();
    pUICameraObj->Camera()->SetLayerCheck(NUM_LAYER_UI, true);

    // Tool Camera가 바라보고 있는 위치에 생성
    CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
    Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
    pUICameraObj->Transform()->SetLocalPosition(Vector3(0.f,0.f,0.f));
    CObject::CreateGameObjectEvn(pUICameraObj, NUM_LAYER_UI);
    return pUICameraObj;
}

void MainMenuGUI::CreateTextUI()
{   
    tstring strObjName = _CreateObjectName(_T("TextUI"), g_iTextUIGameObjectID);
    
    // 새 게임 오브젝트 생성
    CGameObject* pTextUIObj = _CreateUIGameObject();
    pTextUIObj->SetName(strObjName);
    pTextUIObj->AddComponent<CTextUI>();
    pTextUIObj->TextUI()->SetText(_T("Text"));

    // 자식 오브젝트로 넣기
    
    CGameObject* pUICamera = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(STR_OBJ_NAME_UICamera, NUM_LAYER_UI);

    // UI 카메라가 없으면 새로 생성한다.
    if (!pUICamera)
        pUICamera = _CreateDefaultUICamera();

    // UI Camera의 자식 오브젝트로 넣는다.
    CObject::CreateGameObjectEvn(pTextUIObj, NUM_LAYER_UI);
    CObject::AddChildGameObjectEvn(pUICamera, pTextUIObj);
}

void MainMenuGUI::CreateImageUI()
{
    tstring strObjName = _CreateObjectName(_T("ImageUI"), g_iImageUIGameObjectID);

    // 새 게임 오브젝트 생성
    CGameObject* pImageUI = _CreateUIGameObject();
    pImageUI->SetName(strObjName);
    pImageUI->AddComponent<CImageUI>();

    // 자식 오브젝트로 넣기

    CGameObject* pUICamera = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(STR_OBJ_NAME_UICamera, NUM_LAYER_UI);

    // UI 카메라가 없으면 새로 생성한다.
    if (!pUICamera)
        pUICamera = _CreateDefaultUICamera();

    // UI Camera의 자식 오브젝트로 넣는다.
    CObject::CreateGameObjectEvn(pImageUI, NUM_LAYER_UI);
    CObject::AddChildGameObjectEvn(pUICamera, pImageUI);
}

void MainMenuGUI::CreateButtonUI()
{
    tstring strObjName = _CreateObjectName(_T("ButtonUI"), g_iButtonUIGameObjectID);

    // 새 게임 오브젝트 생성
    CGameObject* pButtonUIObj = _CreateUIGameObject();
    pButtonUIObj->SetName(strObjName);
    pButtonUIObj->AddComponent<CButtonUI>();

    // 자식 오브젝트로 넣기

    CGameObject* pUICamera = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(STR_OBJ_NAME_UICamera, NUM_LAYER_UI);

    // UI 카메라가 없으면 새로 생성한다.
    if (!pUICamera)
        pUICamera = _CreateDefaultUICamera();
    
    // UI Camera의 자식 오브젝트로 넣는다.
    CObject::CreateGameObjectEvn(pButtonUIObj, NUM_LAYER_UI);
    CObject::AddChildGameObjectEvn(pUICamera, pButtonUIObj);
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
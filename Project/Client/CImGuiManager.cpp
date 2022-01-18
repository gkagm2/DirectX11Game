#include "pch.h"
#include "CImGuiManager.h"

#include <Engine\CCore.h>
#include <Engine\CDevice.h>
#include <Engine\CKeyManager.h>
#include <Engine\CSceneManager.h>
#include <Engine\CGameObject.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GUI.h"
#include "ListViewGUI.h"
#include "DebugGUI.h"
#include "CollisionEditorGUI.h"



CImGuiManager::CImGuiManager() :
    m_bDemoGUIOpen(false),
    m_iTestCodeType(1),
    m_iID(0),
    m_bOpenGUI(true)
{
}

CImGuiManager::~CImGuiManager()
{
    Safe_Delete_Map(m_mapGUI);
}

void CImGuiManager::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

#pragma region 한글 폰트를 이용하여 인게임에서 입력하기 위한 방법.
            // io.Fonts->AddFontFromFileTTF(/*폰트 경로*/, /*폰트 크기*/, nullptr, io.Fonts->GetGlyphRangesKorean());  
#pragma endregion


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(CCore::GetInstance()->GetWndHandle());
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());


    //ImGuiInitTestCode();
    InitCaptainForeverGUI();
    InitButcherGUI();

    CreateGUI();
}

void CImGuiManager::Progress()
{
    if (InputKeyPress(E_Key::F8))
        m_bOpenGUI = !m_bOpenGUI;

    if (m_bOpenGUI) {
        _InitFrame();
        Update();
        Render();
    }
}

void CImGuiManager::Update()
{
    if (InputKeyPress(E_Key::F1))
        m_bDemoGUIOpen = !m_bDemoGUIOpen;

    // SetTitileButton the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (m_bDemoGUIOpen)
        ImGui::ShowDemoWindow(&m_bDemoGUIOpen);

    // GUI Update
    for (const auto& pair : m_mapGUI) {
        if(pair.second->IsActive()) 
            pair.second->Update();
    }
}

void CImGuiManager::Render()
{
    // Rendering

    // 엔진 내부 버퍼에 그려지는 윈도우즈 UI들 
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // UI가 윈도우 영역 밖으로 나갈 시 별도로 추가된 윈도우즈 창을 업데이트하고 렌더링한다.
    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImGuiManager::CreateGUI()
{
}

GUI* CImGuiManager::FindGUI(const string& _strKey)
{
    map<string, GUI*>::iterator iter = m_mapGUI.find(_strKey);
    if (iter == m_mapGUI.end())
        return nullptr;

    return iter->second;
}

void CImGuiManager::AddGUI(const string& _strName, GUI* _pGUI)
{
    m_mapGUI.insert(std::make_pair(_strName, _pGUI));
}

// Test code
#include "TransformGUI.h"
#include "MeshRendererGUI.h"
#include "InspectorGUI.h"
#include "HierarchyGUI.h"
#include "ResourceGUI.h"
#include "Collider2DGUI.h"
#include "ResourceViewGUI.h"
#include "MenuGUI.h"
#include "MainMenuGUI.h"
#include "SceneViewGUI.h"
#include "TileMapEditorGUI.h"
#include "Animator2DEditorGUI.h"
#include "ToolCameraGUI.h"

#include "LayerCheckGUI.h"

#include "ModuleCreatorGUI_ca.h"

#include "WayPointGUI_bu.h"
#include "SpawningGUI_bu.h"

void CImGuiManager::InitCaptainForeverGUI()
{
    Init_ShowGameObjectComponent();

    ModuleCreatorGUI_ca* pModuleCreatorGUI = new ModuleCreatorGUI_ca;
    pModuleCreatorGUI->Init();
    AddGUI(pModuleCreatorGUI->GetName(), pModuleCreatorGUI);
}

void CImGuiManager::InitButcherGUI()
{
    WayPointGUI_bu* pWPGui = new WayPointGUI_bu;
    pWPGui->Init();
    AddGUI(pWPGui->GetName(), pWPGui);

    SpawningGUI_bu* pSGui = new SpawningGUI_bu;
    pSGui->Init();
    AddGUI(pSGui->GetName(), pSGui);
}

#include "CToolCameraScript.h"
#include <Engine\CRenderManager.h>
void CImGuiManager::SetToolInteractionActive(bool _bActive)
{
    // Tool camera movement 작동 여부
    CCamera* pCamera = CRenderManager::GetInstance()->GetToolCamera();
    CToolCameraScript* pToolCamScript = nullptr;
    if (pCamera) {
        pToolCamScript = pCamera->GetGameObject()->GetComponent<CToolCameraScript>();
        if (pToolCamScript)
            pToolCamScript->SetMovementActive(_bActive);
    }
}


void CImGuiManager::ImGuiInitTestCode()
{
    if (0 == m_iTestCodeType) {
        // Text Print test
    }
    else if (1 == m_iTestCodeType) {
        Init_ShowGameObjectComponent();
    }
    else if (2 == m_iTestCodeType) {
        Init_ListViewGUI();
    }
    else if (3 == m_iTestCodeType) {
        Init_ShowResAndInspectorGUI();
    }
    else if (4 == m_iTestCodeType) {
        Init_ShowMainMenuGUI();
    }
}

void CImGuiManager::Update_PrintTextTest()
{
    ImGui::Begin("Another Windows");
    ImGui::Text("This");
    ImGui::End();
}

void CImGuiManager::Init_ShowGameObjectComponent()
{
    // Resource들을 보여주는 GUI
    ResourceViewGUI* pResourceGUI = new ResourceViewGUI;
    pResourceGUI->Init();
    AddGUI(pResourceGUI->GetName(), pResourceGUI);
    

    // Hierachy GUI
    HierarchyGUI* pHierarchyGUI = new HierarchyGUI;
    pHierarchyGUI->Init();
    AddGUI(pHierarchyGUI->GetName(), pHierarchyGUI);

    // Component들을 보여주는 GUI
    InspectorGUI* pInspectorGUI = new InspectorGUI;
    pInspectorGUI->Init();
    AddGUI(pInspectorGUI->GetName(), pInspectorGUI);
                         
    // 리스트 뷰 GUI
    ListViewGUI* pListView = new ListViewGUI;
    AddGUI(pListView->GetName(), pListView);

    // Main Menu GUI
    MainMenuGUI* pMainMenuGUI = new MainMenuGUI;
    pMainMenuGUI->Init();
    AddGUI(pMainMenuGUI->GetName(), pMainMenuGUI);

    // Debug GUI
    DebugGUI* pDebugGUI = new DebugGUI;
    pDebugGUI->Init();
    AddGUI(pDebugGUI->GetName(), pDebugGUI);

    // Scene View GUI
    SceneViewGUI* pSceneViewGUI = new SceneViewGUI;
    pSceneViewGUI->Init();
    AddGUI(pSceneViewGUI->GetName(), pSceneViewGUI);

    // Tile Map Editor GUI
    TileMapEditorGUI* pTileMapEditorGUI = new TileMapEditorGUI;
    pTileMapEditorGUI->Init();
    AddGUI(pTileMapEditorGUI->GetName(), pTileMapEditorGUI);

    // Animator2d Editor GUI
    Animator2DEditorGUI* pAnimator2DEditorGUI = new Animator2DEditorGUI;
    pAnimator2DEditorGUI->Init();
    AddGUI(pAnimator2DEditorGUI->GetName(), pAnimator2DEditorGUI);

    // Tool Camera GUI
    ToolCameraGUI* pToolCameraGUI = new ToolCameraGUI;
    pToolCameraGUI->Init();
    AddGUI(pToolCameraGUI->GetName(), pToolCameraGUI);

    // CollsionEditor GUI
    CollisionEditorGUI* pCollsionEditorGUI = new CollisionEditorGUI;
    pCollsionEditorGUI->Init();
    AddGUI(pCollsionEditorGUI->GetName(), pCollsionEditorGUI);

    LayerCheckGUI* pLayerCheckGUI = new LayerCheckGUI();
    pLayerCheckGUI->Init();
    AddGUI(pLayerCheckGUI->GetName(), pLayerCheckGUI);

}

void CImGuiManager::Init_ListViewGUI()
{
    ListViewGUI* pListView = new ListViewGUI;
    pListView->SetName("ListView");
    AddGUI(pListView->GetName(), pListView);
}

void CImGuiManager::Init_ShowResAndInspectorGUI()
{
    // ResourceGUI
    ResourceViewGUI* pGUI = new ResourceViewGUI;
    pGUI->Init();
    AddGUI(pGUI->GetName(), pGUI);

    // InspectorGUI
    InspectorGUI* pInspectorGUI = new InspectorGUI;
    pInspectorGUI->Init();
    AddGUI(pInspectorGUI->GetName(), pInspectorGUI);

    // ListGUI
    ListViewGUI* pListViewGUI = new ListViewGUI;
    pListViewGUI->Init();
    AddGUI(pListViewGUI->GetName(), pListViewGUI);
}

void CImGuiManager::Init_ShowMainMenuGUI()
{
    MainMenuGUI* pMainMenuGUI = new MainMenuGUI;
    pMainMenuGUI->Init();
    AddGUI(pMainMenuGUI->GetName(), pMainMenuGUI);
}

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



CImGuiManager::CImGuiManager() :
    m_bDemoGUIOpen(false),
    m_iTestCodeType(1)
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

#pragma region �ѱ� ��Ʈ�� �̿��Ͽ� �ΰ��ӿ��� �Է��ϱ� ���� ���.
            // io.Fonts->AddFontFromFileTTF(/*��Ʈ ���*/, /*��Ʈ ũ��*/, nullptr, io.Fonts->GetGlyphRangesKorean());  
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

    ImGuiInitTestCode();
    CreateGUI();
}

void CImGuiManager::Progress()
{
    Update();
    Render();
}

void CImGuiManager::Update()
{
    if (InputKeyPress(E_Key::F1))
        m_bDemoGUIOpen = !m_bDemoGUIOpen;

    // Start the Dear ImGui frame
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

    // ���� ���� ���ۿ� �׷����� �������� UI�� 
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // UI�� ������ ���� ������ ���� �� ������ �߰��� �������� â�� ������Ʈ�ϰ� �������Ѵ�.
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
#include "HierachyGUI.h"
#include "ResourceGUI.h"
#include "Collider2DRectGUI.h"
#include "ResourceManagerGUI.h"
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
    CGameObject* pGameObject = CSceneManager::GetInstance()->FindGameObject(_T("Player"));
    assert(pGameObject); // TestScene���� Player�� �̸��� ���� ������Ʈ�� �ϳ� ������

    // Resource���� �����ִ� GUI
    ResourceManagerGUI* pResourceGUI = new ResourceManagerGUI;
    pResourceGUI->Init();
    AddGUI(pResourceGUI->GetName(), pResourceGUI);
    

    // Hierachy GUI
    HierachyGUI* pHierachyGUI = new HierachyGUI;
    pHierachyGUI->Init();
    AddGUI(pHierachyGUI->GetName(), pHierachyGUI);

    // Component���� �����ִ� GUI
    InspectorGUI* pInspectorGUI = new InspectorGUI;
    pInspectorGUI->Init();
    AddGUI(pInspectorGUI->GetName(), pInspectorGUI);
                         
    // ����Ʈ �� GUI
    ListViewGUI* pListView = new ListViewGUI;
    AddGUI(pListView->GetName(), pListView);
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
    ResourceManagerGUI* pGUI = new ResourceManagerGUI;
    pGUI->Init();
    AddGUI(pGUI->GetName(), pGUI);

    // InspectorGUI
    InspectorGUI* pInspectorGUI = new InspectorGUI;
    pInspectorGUI->Init();
    AddGUI(pInspectorGUI->GetName(), pInspectorGUI);

    // ListGUI
    ListViewGUI* pListViewGUI = new ListViewGUI;
    pListViewGUI->Init();
    m_mapGUI.insert(make_pair(pListViewGUI->GetName(), pListViewGUI));
}
#include "pch.h"
#include "ModuleCreatorGUI_ca.h"
#include <Script\CModuleCreator_ca.h>
#include "MainMenuGUI.h"
#include "CImGuiManager.h"
#include "CTestScene.h"
#include <Script\CModuleScript_ca.h>
#include <Script\CCommandModuleScript_ca.h>
#include <Script\CGirder1x2Script_ca.h>
#include <Engine\CGameObject.h>
#include <Engine\CCamera.h>
#include <Engine\CRenderManager.h>
#include <Engine\CCore.h>
#include "DebugGUI.h"

#include "TextureGUI.h"

ModuleCreatorGUI_ca::ModuleCreatorGUI_ca() :
    m_pModuleCreator(nullptr)
{
	SetName(STR_GUI_ModuleCreator);
}

ModuleCreatorGUI_ca::~ModuleCreatorGUI_ca()
{
}
void ModuleCreatorGUI_ca::Init()
{
    // ��� ����Ʈ �̸� ����
    UINT iCnt = (UINT)E_ModuleType_ca::End;
    for (UINT i = 0; i < iCnt; ++i) {
        string strName = {};
        TStringToString(ModuleTypeSTR_ca[i], strName);
        m_vecModuleTypeName.push_back(strName);
    }

    // ��� ���� �̸� ����
    iCnt = (UINT)E_ModuleLevel_ca::End;
    for (UINT i = 0; i < iCnt; ++i) {
        string strName = {};
        TStringToString(ModuleLevelSTR_ca[i], strName);
        m_vecModuleLevelName.push_back(strName);
    }

    // Prefab ���� ����
    iCnt = (UINT)E_ModuleType_ca::End;
    m_vecPrefabKey.resize(iCnt);
    m_vecPrefabKey[(UINT)E_ModuleType_ca::Girder1x1] = "prefab\\Module_Girder1x1.pref";
    m_vecPrefabKey[(UINT)E_ModuleType_ca::Girder1x2] = "prefab\\Module_Girder1x2.pref";
    m_vecPrefabKey[(UINT)E_ModuleType_ca::Laser] = "prefab\\Module_Laser.pref";
    m_vecPrefabKey[(UINT)E_ModuleType_ca::Booster] = "prefab\\Module_Booster.pref";
    m_vecPrefabKey[(UINT)E_ModuleType_ca::Command] = "prefab\\Module_Command.pref";

    vector<tstring> vecName;
    CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Prefab, vecName);

    for (UINT i = 0; i < vecName.size(); ++i) {
        _tcprintf(_T("Prefab [%s]\n"), vecName[i].c_str());
    }
}

void ModuleCreatorGUI_ca::Update()
{
	// â�� �ϳ� ����
    if (!m_bGUIOpen) {
        if (m_pModuleCreator) {
            CObject::DestroyGameObjectEvn(m_pModuleCreator->GetGameObject());
            m_pModuleCreator = nullptr;
        }
        return;
    }
        
    // Module Creator�� ������
    /*if (nullptr == m_pModuleCreator) {
        CGameObject* pModuleCreatorObj = FIND_GameObject(_T("ModuleCreator"));
        if (nullptr == pModuleCreatorObj) {
            MainMenuGUI* pMainMenuGUI = (MainMenuGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_MainMenu);
            if (nullptr == pMainMenuGUI) {
                assert(nullptr);
                return;
            }
            pModuleCreatorObj = pMainMenuGUI->CreateEmptyGameObject();
            pModuleCreatorObj->AddComponent<CModuleCreator_ca>();
            pModuleCreatorObj->SetName(_T("ModuleCreator"));
        }
        m_pModuleCreator = pModuleCreatorObj->GetComponent<CModuleCreator_ca>();
        CObject::CreateGameObjectEvn(pModuleCreatorObj, 10);
    }*/

    // GUI
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Module Creator", &m_bGUIOpen, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu(""))
            {
                if (ImGui::MenuItem("Close")) {
                    SetActive(false);
                }
                    
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // ���� ����Ʈ�� �̿��Ͽ� Girdoer1x1, Girder1x2, Laser, Booster,Command�� �����ϰڴ�.
        // Left
        static int selectedModule = 0;
        static int selectedModuleLevel = (UINT)E_ModuleLevel_ca::Alpha;
        static int selectedCommandModuleLevel = (UINT)E_ModuleLevel_ca::Nemesis;
        {
            ImGui::Text("Module Type");
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            for (int i = 0; i < (UINT)E_ModuleType_ca::End; ++i) {
                if (ImGui::Selectable(m_vecModuleTypeName[i].c_str(), selectedModule == i)) {
                    selectedModule = i;
                }
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("module view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("Module Type : %s", m_vecModuleTypeName[selectedModule].c_str());

            ImGui::Separator();
            
            // ��� ����
            switch (E_ModuleType_ca(selectedModule)) {
            case E_ModuleType_ca::Girder1x1: // Girder1x1
            case E_ModuleType_ca::Girder1x2: // Girder1x2
            case E_ModuleType_ca::Laser: // Booster
            case E_ModuleType_ca::Booster: { // Booster
                // Module Level ����
                
                {
                    ImGui::Text("Module Level %s", m_vecModuleLevelName[selectedModuleLevel].c_str());
                    ImGui::BeginChild("module level pane##CaptainForever", ImVec2(0, 200), true);
                    for (UINT i = (UINT)E_ModuleLevel_ca::Alpha; i < (UINT)E_ModuleLevel_ca::End; ++i) {
                        if (ImGui::Selectable(m_vecModuleLevelName[i].c_str(), selectedModuleLevel == i)) {
                            selectedModuleLevel = i;
                        }
                    }
                    ImGui::EndChild();
                }
            }
                break;
            case E_ModuleType_ca::Command: {// Command
                // Command Module Level ����
                
                {
                    ImGui::Text("Command Module Level %s", m_vecModuleLevelName[selectedCommandModuleLevel].c_str());
                    ImGui::BeginChild("command module level pane##CaptainForever", ImVec2(0, 200), true);
                    for (UINT i = (UINT)E_ModuleLevel_ca::Nemesis; i < (UINT)E_ModuleLevel_ca::End; ++i) {
                        if (ImGui::Selectable(m_vecModuleLevelName[i].c_str(), selectedCommandModuleLevel == i)) {
                            selectedCommandModuleLevel = i;
                        }
                    }
                    ImGui::EndChild();
                }
                break;
            }
            default:
                assert(nullptr);
                break;
            }

            /*if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }*/
            static int iLayer = 0;
            ImGui::InputInt("Layer ", &iLayer);

            // �����齺 �̹��� �����ֱ�.
            // ��� Ÿ�Ժ��� ���Ͽ��� �̹����� �����ͼ� �����ִ°ɷ� ����
            //ImGuiIO& io = ImGui::GetIO();
            //ImTextureID my_tex_id = io.Fonts->TexID;
            //float my_tex_w = (float)io.Fonts->TexWidth;
            //float my_tex_h = (float)io.Fonts->TexHeight;
            //{
            //    ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
            //    ImVec2 pos = ImGui::GetCursorScreenPos();
            //    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            //    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            //    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            //    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            //    ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
            //    if (ImGui::IsItemHovered())
            //    {
            //        ImGui::BeginTooltip();
            //        float region_sz = 32.0f;
            //        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
            //        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
            //        float zoom = 4.0f;
            //        if (region_x < 0.0f) { region_x = 0.0f; }
            //        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
            //        if (region_y < 0.0f) { region_y = 0.0f; }
            //        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
            //        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
            //        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
            //        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
            //        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
            //        ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
            //        ImGui::EndTooltip();
            //    }
            //}
            ImGui::EndChild();

            iLayer = CMyMath::Clamp(iLayer, 0, MAX_SIZE_LAYER - 1);
            if (ImGui::Button("Add")) {
                tstring strKey {};
                StringToTString(m_vecPrefabKey[selectedModule], strKey);
                // �����齺���� �ҷ�����
                SharedPtr<CPrefab> pPrefab = CResourceManager::GetInstance()->LoadRes<CPrefab>(strKey);

                if (nullptr == pPrefab->GetProtoObj()) {
                    DBug->Debug("[Warning] can't find the prefab file");
                   // assert(nullptr && _T("Prefab�� ã�� �� ����"));
                }
                else {
                    CCamera* pCamera = CRenderManager::GetInstance()->GetToolCamera();
                    Vector2 vRespawnPos = CCore::GetInstance()->GetWindowResolution();
                    vRespawnPos /= 2.f;
                    Vector3 vCreationPosition = pCamera->GetScreenToWorld2DPosition(vRespawnPos);
                    CGameObject* pNewObj = CObject::InstantiateEvn(pPrefab, vCreationPosition, iLayer);

                    CGameObject* pModuleObj = pNewObj->FindGameObjectInChilds(_T("Module"));
                    CModuleScript_ca* pModuleScript = pModuleObj->GetComponent<CModuleScript_ca>();
                    if (nullptr == pModuleScript) {
                        assert(nullptr);
                    }
                    else {
                        UINT iSelectedModuleLevel;
                        if (E_ModuleType_ca::Command == (E_ModuleType_ca)selectedModule)
                            iSelectedModuleLevel = selectedCommandModuleLevel;
                        else
                            iSelectedModuleLevel = selectedModuleLevel;
                        pModuleScript->SetModuleLevel((E_ModuleLevel_ca)iSelectedModuleLevel);
                    }
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Save")) {
                const unordered_map<tstring, CResource*>& res = CResourceManager::GetInstance()->GetResources(E_ResourceType::Prefab);
                vector<tstring> ve;
                CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Prefab, ve);
                for (auto& pref : ve) {
                    _tcprintf(_T("Prefab [%s]\n"), pref.c_str());
                }
            }
            ImGui::EndGroup();
        }
    }
    ImGui::End();
}
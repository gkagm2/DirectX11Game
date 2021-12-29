#include "pch.h"
#include "ListViewGUI.h"
#include <Engine\CKeyManager.h>
#include <Engine\CTimeManager.h>
#include <Engine\CResourceManager.h>
#include "ParamGUI.h"

ListViewGUI::ListViewGUI() :
    m_bPopUp(false),
    m_pDBCCallBack(nullptr),
    m_pInst(nullptr),
    m_dwSecondData(0),
    m_iCurItemIdx(0),
    m_iSelectIdx(0),
    m_pPreViewTex(nullptr)
{
    SetName("ListView");
    SetActive(false);
}

ListViewGUI::~ListViewGUI()
{
}

void ListViewGUI::Update()
{
	// ������Ʈ ����Ʈ �� ����

    // �˾� �÷��װ� on�̸�
    if (m_bPopUp) {
        // �˾�â�� ����
        ImGui::OpenPopup(m_strTitle.c_str());
        m_bPopUp = false;
    }
   /* if (false == ImGui::IsPopupOpen(m_strTitle.c_str()))
        ImGui::OpenPopup(m_strTitle.c_str());*/

    if ("" == m_strTitle.c_str())
        return;

    // ��� �˾�â�� �����.
    static char filter[255]{};

    if (ImGui::BeginPopupModal(m_strTitle.c_str(), &m_bGUIOpen, 0)) {
    //if (ImGui::BeginPopup(m_strTitle.c_str(), ImGuiWindowFlags_AlwaysAutoResize)) {
        if (InputKeyPress(E_Key::ESCAPE))
            m_bGUIOpen = false;

        if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(0);

        bool bIsFirst = false;
        if (ImGui::InputText("##ListBox Filter", filter, 255)) {
            bIsFirst = true;
        }    
        if (m_bIsRenderTexture)
            _UpdateTexture();

        // ����Ʈ�� ǥ��
        ImVec2 vWindowSize = ImGui::GetWindowSize();
        vWindowSize.y * 0.5f;
        vector<int> vecEnableIdxs;
        if (ImGui::BeginListBox("##ListBox", vWindowSize)) {
            // ����Ʈ�� ���� ���ڵ��� ��ȸ�Ͽ� ǥ��
            for (UINT i = 0; i < m_vecListAdr.size(); ++i) {
                // ���ڿ� filter 
                string str = m_vecListAdr[i];
                string strFilter = filter;
                StringTool::StringToLower(str);
                StringTool::StringToLower(strFilter);
                if (strcmp(filter, "") != 0) {
                    if (str.find(strFilter) == string::npos)
                        continue;
                }

                if (bIsFirst) {
                    m_iCurItemIdx = i;
                    m_iSelectIdx = 0;
                    bIsFirst = false;
                }
                vecEnableIdxs.push_back(i);
            }
            
            for (UINT i = 0; i < vecEnableIdxs.size(); ++i) {
                int idx = vecEnableIdxs[i];
                bool is_selected = (m_iCurItemIdx == idx);
                if (ImGui::Selectable(m_vecListAdr[idx], is_selected)) {
                    m_iSelectIdx = i;
                    m_iCurItemIdx = vecEnableIdxs[m_iSelectIdx];
                }
                // �޺��� �� �� �ʱ� ��Ŀ�� ����(��ũ�Ѹ� + Ű���� Ž�� ��Ŀ��)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            

            static float fDelayTime = 0;
            if (InputKeyPress(E_Key::UP) || ImGui::IsItemHovered()) {
                --m_iSelectIdx;
                m_iSelectIdx = CMyMath::Clamp(m_iSelectIdx, 0, max(0, (int)(vecEnableIdxs.size() - 1)));
                m_iCurItemIdx = vecEnableIdxs[m_iSelectIdx];
                fDelayTime = -0.3f;
            }
            if (InputKeyPress(E_Key::DOWN) || ImGui::IsItemHovered()) {
                ++m_iSelectIdx;
                m_iSelectIdx = CMyMath::Clamp(m_iSelectIdx, 0, max(0, (int)(vecEnableIdxs.size() - 1)));
                m_iCurItemIdx = vecEnableIdxs[m_iSelectIdx];
                fDelayTime = -0.3f;
            }

            constexpr float fMaxDelay = 0.06f;
            
            if (InputKeyHold(E_Key::UP) || ImGui::IsItemHovered()) {
                fDelayTime += DT;
                if (fDelayTime >= fMaxDelay) {
                    --m_iSelectIdx;
                    m_iSelectIdx = CMyMath::Clamp(m_iSelectIdx, 0, max(0, (int)(vecEnableIdxs.size() - 1)));
                    m_iCurItemIdx = vecEnableIdxs[m_iSelectIdx];
                    fDelayTime = 0.f;
                }
            }
            if (InputKeyHold(E_Key::DOWN) || ImGui::IsItemHovered()) {
                fDelayTime += DT;
                if (fDelayTime >= fMaxDelay) {
                    ++m_iSelectIdx;
                    m_iSelectIdx = CMyMath::Clamp(m_iSelectIdx, 0, max(0, (int)(vecEnableIdxs.size() - 1)));
                    m_iCurItemIdx = vecEnableIdxs[m_iSelectIdx];
                    fDelayTime = 0.f;
                }
            }

            if (ImGui::IsMouseDoubleClicked(0)) {
                
                // �ݹ� �Լ��� ȣ��
                if (m_pInst && m_pDBCCallBack)
                    ((*m_pInst).*m_pDBCCallBack)((DWORD_PTR)m_vecListAdr[m_iCurItemIdx], m_dwSecondData);

                if (m_pGDBCCallBack)
                    m_pGDBCCallBack((DWORD_PTR)m_vecListAdr[m_iCurItemIdx], m_dwSecondData);
                memset(filter, 0, 255);
                ImGui::CloseCurrentPopup();
                _Clear();
            }

            // �������� ����Ŭ�� ���� ���
            if (InputKeyPress(E_Key::Enter))
            {
                // �ݹ� �Լ��� ȣ��
                if (m_pInst && m_pDBCCallBack)
                    ((*m_pInst).*m_pDBCCallBack)((DWORD_PTR)m_vecListAdr[m_iCurItemIdx], m_dwSecondData);
                 
                if (m_pGDBCCallBack)
                    m_pGDBCCallBack((DWORD_PTR)m_vecListAdr[m_iCurItemIdx], m_dwSecondData);

                memset(filter, 0, 255);
                ImGui::CloseCurrentPopup();
                _Clear();
            }
            ImGui::EndListBox();
        }
        ImGui::EndPopup();





     }
    else {
        memset(filter, 0, 255);
        _Clear();
    }
}

void ListViewGUI::SetList(const vector<tstring>& _vecNames, const tstring& _strTitle)
{
    // Set Title
    TStringToString(_strTitle, m_strTitle);

    // ����Ʈ�� ���ڿ� ����
    m_vecListName.clear();
    string strName;
    for (UINT i = 0; i < _vecNames.size(); ++i) {
        TStringToString(_vecNames[i], strName);
        m_vecListName.push_back(strName);
    }

    // ������ ���ڿ� ����
    m_vecListAdr.clear();
    for (const auto& name : m_vecListName)
        m_vecListAdr.push_back(name.c_str());
}

void ListViewGUI::SetActive(bool _bIsActive)
{
    GUI::SetActive(_bIsActive);
    _SetActivePopup(_bIsActive);
}

void ListViewGUI::_Clear()
{
    m_pInst = nullptr;
    m_pDBCCallBack = nullptr;
    m_pGDBCCallBack = nullptr;

    m_vecListName.clear();
    m_vecListAdr.clear();
    m_iCurItemIdx = 0;
    m_iSelectIdx = 0;

    ImGui::SetWindowFocus(nullptr);

    // Ȯ��
    m_bIsRenderTexture = false;

    

    SetActive(false);
}


void ListViewGUI::_UpdateTexture()
{
    // ������ �ؽ��ĸ� �˾Ƴ���.
    string str = m_vecListAdr[m_iCurItemIdx];
    tstring tStr;
    StringToTString(str, tStr);
    m_pPreViewTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStr).Get();
    assert(m_pPreViewTex);

    ParamGUI::Render_Texture("PreView", m_pPreViewTex, nullptr, nullptr, false);
}
#include "pch.h"
#include "ListViewGUI.h"
#include <Engine\CKeyManager.h>

ListViewGUI::ListViewGUI() :
    m_bPopUp(false),
    m_pDBCCallBack(nullptr),
    m_pInst(nullptr),
    m_dwSecondData(0)
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
    if (ImGui::BeginPopupModal(m_strTitle.c_str(), &m_bGUIOpen)) {
    //if (ImGui::BeginPopup(m_strTitle.c_str(), ImGuiWindowFlags_AlwaysAutoResize)) {

        if (InputKeyPress(E_Key::ESCAPE))
            m_bGUIOpen = false;

        if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(0);
        ImGui::InputText("##ListBox Filter", filter,255);

        static int item_current_idx = 0; // ������ �������� �ε���
        // ����Ʈ�� ǥ��
        ImVec2 vWindowSize = ImGui::GetWindowSize();
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

                bool is_selected = (item_current_idx == i);
                if (ImGui::Selectable(m_vecListAdr[i], is_selected))
                    item_current_idx = i;

                // �޺��� �� �� �ʱ� ��Ŀ�� ����(��ũ�Ѹ� + Ű���� Ž�� ��Ŀ��)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();

                // �������� ����Ŭ�� ���� ���
                if (InputKeyPress(E_Key::Enter) ||
                    ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    // �ݹ� �Լ��� ȣ��
                    if (m_pInst && m_pDBCCallBack)
                        ((*m_pInst).*m_pDBCCallBack)((DWORD_PTR)m_vecListAdr[i], m_dwSecondData);

                    if (m_pGDBCCallBack)
                        m_pGDBCCallBack((DWORD_PTR)m_vecListAdr[i], m_dwSecondData);
                    memset(filter, 0, 255);
                    ImGui::CloseCurrentPopup();
                    
                    _Clear();
                }
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

    m_vecListName.clear();
    m_vecListAdr.clear();

    ImGui::SetWindowFocus(nullptr);

    SetActive(false);
}
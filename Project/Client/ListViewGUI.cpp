#include "pch.h"
#include "ListViewGUI.h"

ListViewGUI::ListViewGUI() :
    m_bPopUp(false),
    m_pDBCCallBack(nullptr),
    m_pInst(nullptr)
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
    if (ImGui::BeginPopupModal(m_strTitle.c_str(), &m_bGUIOpen, ImGuiWindowFlags_None)) {
        ImGui::SetNextWindowSize(ImVec2(300.f, 500.f));


        static int item_current_idx = 0; // ������ �������� �ε���
        // ����Ʈ�� ǥ��
        if (ImGui::BeginListBox("##ListBox")) {
            

            // ����Ʈ�� ���� ���ڵ��� ��ȸ�Ͽ� ǥ��
            for (UINT i = 0; i < m_vecListAdr.size(); ++i) {

                const bool is_selected = (item_current_idx == i);
                if (ImGui::Selectable(m_vecListAdr[i], is_selected))
                    item_current_idx = i;

                // �޺��� �� �� �ʱ� ��Ŀ�� ����(��ũ�Ѹ� + Ű���� Ž�� ��Ŀ��)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();

                // �������� ����Ŭ�� ���� ���
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    // �ݹ� �Լ��� ȣ��
                    if (m_pInst && m_pDBCCallBack)
                        ((*m_pInst).*m_pDBCCallBack)((DWORD_PTR)m_vecListAdr[i], 0);

                    ImGui::CloseCurrentPopup();
                    _Clear();
                }
            }

            ImGui::EndListBox();
        }
        ImGui::EndPopup();
     }
    else {
        _Clear();
    }
}

void ListViewGUI::SetList(const vector<tstring>& _vecNames, const tstring& _strTitle)
{
    // Set Title
#ifdef UNICODE
    WStringToString(_strTitle, m_strTitle);
#elif
    m_tTitle = _strTitle;
#endif

    // ����Ʈ�� ���ڿ� ����
    m_vecListName.empty();
#ifdef UNICODE
    string strName;
    for (UINT i = 0; i < _vecNames.size(); ++i) {
        WStringToString(_vecNames[i], strName);
        m_vecListName.push_back(strName);
    }
#elif
    m_vecListName = _vecNames;
#endif

    // ������ ���ڿ� ����
    m_vecListAdr.empty();
    for (const auto& name : m_vecListName)
        m_vecListAdr.push_back(name.c_str());
}

void ListViewGUI::SetActive(bool _bIsActive)
{
    GUI::SetActive(_bIsActive);
    _SetActivePopup(_bIsActive);
    m_bGUIOpen = true;
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
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
	// 컴포넌트 리스트 뷰 생성

    // 팝업 플래그가 on이면
    if (m_bPopUp) {
        // 팝업창을 연다
        ImGui::OpenPopup(m_strTitle.c_str());
        m_bPopUp = false;
    }
   /* if (false == ImGui::IsPopupOpen(m_strTitle.c_str()))
        ImGui::OpenPopup(m_strTitle.c_str());*/

    if ("" == m_strTitle.c_str())
        return;

    // 모달 팝업창을 만든다.
    if (ImGui::BeginPopupModal(m_strTitle.c_str(), &m_bGUIOpen, ImGuiWindowFlags_None)) {
        ImGui::SetNextWindowSize(ImVec2(300.f, 500.f));


        static int item_current_idx = 0; // 선택한 데이터의 인덱스
        // 리스트를 표시
        if (ImGui::BeginListBox("##ListBox")) {
            

            // 리스트에 적을 글자들을 순회하여 표시
            for (UINT i = 0; i < m_vecListAdr.size(); ++i) {

                const bool is_selected = (item_current_idx == i);
                if (ImGui::Selectable(m_vecListAdr[i], is_selected))
                    item_current_idx = i;

                // 콤보를 열 때 초기 포커스 설정(스크롤링 + 키보드 탐색 포커스)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();

                // 아이템을 더블클릭 했을 경우
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    // 콜백 함수를 호출
                    if (m_pInst && m_pDBCCallBack)
                        ((*m_pInst).*m_pDBCCallBack)((DWORD_PTR)m_vecListAdr[i], 0);

                    if (m_pGDBCCallBack)
                        m_pGDBCCallBack((DWORD_PTR)m_vecListAdr[i], 0);

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
    TStringToString(_strTitle, m_strTitle);

    // 리스트의 문자열 세팅
    m_vecListName.empty();
    string strName;
    for (UINT i = 0; i < _vecNames.size(); ++i) {
        TStringToString(_vecNames[i], strName);
        m_vecListName.push_back(strName);
    }

    // 보여줄 문자열 세팅
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
#pragma once
#include "GUI.h"

typedef void (GUI::* GUI_CALLBACK)(DWORD_PTR, DWORD_PTR);
typedef void (*GLOBAL_CALLBACK)(DWORD_PTR, DWORD_PTR);
class ListViewGUI : public GUI
{
private:
	string m_strTitle;
	vector<string> m_vecListName;
	vector<const char*> m_vecListAdr; // 리스트에 보여줄 문자열들

	bool m_bPopUp;

	// 더블클릭 됐을 때 호출할 멤버함수 포인터
	GUI_CALLBACK m_pDBCCallBack; // DBC : Double Click
	GUI* m_pInst;

	GLOBAL_CALLBACK m_pGDBCCallBack; // GDBC : global double click
	
public:
	virtual void Update() override;

public:
	void SetList(const vector<tstring>& _vecNames, const tstring& _strTitle);
	inline virtual void SetActive(bool _bIsActive) override;
	void SetDoubleClickCallBack(GUI* _pGUI, GUI_CALLBACK _pFunc) {
		m_pInst = _pGUI; m_pDBCCallBack = _pFunc;
	}

	void SetDoubleClickCallBack(GLOBAL_CALLBACK _pFunc) {
		m_pGDBCCallBack = _pFunc;
	}

private:
	void _SetActivePopup(bool _bIsActive) { m_bPopUp = _bIsActive; }
	void _Clear();

public:
	ListViewGUI();
	virtual ~ListViewGUI() override;
};
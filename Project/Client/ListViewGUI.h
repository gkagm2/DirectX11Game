#pragma once
#include "GUI.h"

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

	DWORD_PTR m_dwSecondData;
	int m_iCurItemIdx;
	int m_iSelectIdx;


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
	void SetSecondData(DWORD_PTR _dwSecondData) { m_dwSecondData = _dwSecondData; }

private:
	void _SetActivePopup(bool _bIsActive) { m_bPopUp = _bIsActive; }
	void _Clear();

	void _OnUpKey(int& iSelectIdx, int maxSize);
	void _OnDownKey();

public:
	ListViewGUI();
	virtual ~ListViewGUI() override;
};
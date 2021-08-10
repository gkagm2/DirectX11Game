#pragma once
#include "GUI.h"

typedef void (GUI::*GUI_CALLBACK)(DWORD_PTR, DWORD_PTR);
typedef void (*GLOBAL_CALLBACK)(DWORD_PTR, DWORD_PTR);
class ContextMenuGUI : public GUI
{
protected:
	vector<string> m_vecListName;
	vector<const char*> m_vecListAdr; // 리스트에 보여줄 문자열들

	bool m_bPopUp;

	// 클릭 됐을 때 호출할 멤버함수 포인터
	GUI_CALLBACK m_pClickCallBack;
	GUI* m_pInst;

	GLOBAL_CALLBACK m_pGClickCallBack; // global click

public:
	virtual void Update() override;

protected:
	void SetPopUp(bool _bActive) { m_bPopUp = _bActive; }
	bool IsPopUpOpen() { return m_bPopUp; }

public:
	void SetList(const vector<tstring>& _vecNames);
	inline virtual void SetActive(bool _bIsActive) override;
	void SetClickCallBack(GUI* _pGUI, GUI_CALLBACK _pFunc) {
		m_pInst = _pGUI; m_pClickCallBack = _pFunc;
	}

	void SetClickCallBack(GLOBAL_CALLBACK _pFunc) {
		m_pGClickCallBack = _pFunc;
	}

private:
	void _SetActiveUI(bool _bIsActive) { m_bPopUp = _bIsActive; }
	virtual void _Clear() {};

public:
	ContextMenuGUI();
	virtual ~ContextMenuGUI() override;
};
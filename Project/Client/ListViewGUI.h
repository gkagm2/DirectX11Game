#pragma once
#include "GUI.h"
class ListViewGUI : public GUI
{
private:
	string m_strTitle;
	vector<string> m_vecListName;
	vector<const char*> m_vecListAdr; // 리스터에 보여줄 문자열들

	bool m_bPopUp;

public:
	virtual void Update() override;

public:
	void SetList(const vector<tstring>& _vecNames, const tstring& _strTitle);

	inline virtual void SetActive(bool _bIsActive) override;

private:
	void _SetActivePopup(bool _bIsActive) { m_bPopUp = _bIsActive; }
	void _Clear();

public:
	ListViewGUI();
	virtual ~ListViewGUI() override;
};
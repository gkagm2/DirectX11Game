#pragma once
#include "GUI.h"
class SpawningGUI_bu : public GUI
{
private:
	int m_iCurCategoryItemIdx;
	vector<string> m_CategoryList;
	vector<string> m_StrList;

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	void _Clear();

public:
	SpawningGUI_bu();
	virtual ~SpawningGUI_bu() override;
};
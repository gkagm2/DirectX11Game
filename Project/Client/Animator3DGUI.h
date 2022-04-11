#pragma once
#include "AnimatorGUI.h"
class Animator3DGUI : public AnimatorGUI
{
private:
	vector<char> m_strList;

public:
	virtual void Update() override;

private:
	void _InitComboBox();
public:
	Animator3DGUI();
	virtual ~Animator3DGUI() override;
};
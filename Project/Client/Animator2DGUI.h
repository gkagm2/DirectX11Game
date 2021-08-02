#pragma once
#include "AnimatorGUI.h"
class Animator2DGUI : public AnimatorGUI
{
	vector<char> m_strList;
public:
	virtual void Update() override;

private:
	void _InitComboBox();
public:
	Animator2DGUI();
	virtual ~Animator2DGUI() override;
};
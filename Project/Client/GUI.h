#pragma once
#include "imgui.h"
class GUI
{
private:
	string m_strName;

protected:
	bool m_bGUIOpen;
public:
	virtual void Update() = 0;

	void SetName(const string& _strName) { m_strName = _strName; }
	const string& GetName() { return m_strName; }
	
public:
	GUI();
	virtual ~GUI();
};
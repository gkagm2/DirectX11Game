#pragma once
#include "imgui.h"
class GUI
{
private:
	string m_strName;
	bool m_bActive;

protected:
	bool m_bGUIOpen;

public:
	virtual void Init() {}
	virtual void Update() = 0;

	void SetName(const string& _strName) { m_strName = _strName; }
	const string& GetName() { return m_strName; }

public:
	virtual void SetActive(bool _bIsActive) { m_bActive = _bIsActive; }
	bool IsActive() { return m_bActive; }
	static void GetResourceName(CResource* _pResource, string& _pOut);
	static void GetResourceName(CResource* _pResource, char* _pOut, int _size);
public:
	GUI();
	virtual ~GUI();
};
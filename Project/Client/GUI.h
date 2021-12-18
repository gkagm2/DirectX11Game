#pragma once
#include "imgui.h"


class GUI;
// 더블 클릭 시 콜백
typedef void (GUI::* GUI_CALLBACK)(DWORD_PTR, DWORD_PTR);
typedef void (*GLOBAL_CALLBACK)(DWORD_PTR, DWORD_PTR);

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
	virtual void SetActive(bool _bIsActive) { m_bActive = _bIsActive; m_bGUIOpen = _bIsActive; }
	bool IsActive() { return m_bActive; }
	static void GetResourceName(CResource* _pResource, string& _pOut);
	static void GetResourceName(CResource* _pResource, char* _pOut, int _size);

	static bool IsMouseInWindowContentRegion();

	static void ChangeStateEvn();

public:
	GUI();
	virtual ~GUI();
};
#pragma once
class GUI;
class CImGuiManager : public CSingleton<CImGuiManager>
{
	SINGLETON(CImGuiManager)

private:
	map<string, GUI*> m_mapGUI;
	bool m_bDemoGUIOpen;

public:
	void Init();
	void Progress();

private:
	void Update();
	void Render();
	void CreateGUI();

public:
	GUI* FindGUI(const string& _strKey);
	void AddGUI(const string& _strName, GUI* _pGUI);

public:
	void InitCaptainForeverGUI();

private:
	// Test Codes
	int m_iTestCodeType;

	void ImGuiInitTestCode();

	// testCode 0. Text Print
	void Update_PrintTextTest();

	// testCode 1. Show GameObject Component's GUI
	void Init_ShowGameObjectComponent();

	// testCode 2. Show ListView GUI
	void Init_ListViewGUI();

	// testCode 3. ResourceGUI & InspectorGUI
	void Init_ShowResAndInspectorGUI();

	// testCode 4. MainMenu GUI
	void Init_ShowMainMenuGUI();
};
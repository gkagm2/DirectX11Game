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

private:
	// Test Codes
	int m_iTestCodeType;
	void ImGuiInitTestCode();
	void ImGuiUpdateTestCode();


	void Update_PrintTextTest();

	// testCode 1. Show GameObject Component's GUI
	void Init_ShowGameObjectComponent();
	void Update_ShowGameObjectComponent();

};
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
	void ImGuiTestCode();
	void PrintTextTest();
};
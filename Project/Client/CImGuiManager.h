#pragma once
class CImGuiManager : public CSingleton<CImGuiManager>
{
	SINGLETON(CImGuiManager)

private:
	//map<tstring, CimGui*> m_mapGUI;


public:
	void Init();
	void Progress();

private:
	void Update();
	void Render();
	void CreateGUI();
};
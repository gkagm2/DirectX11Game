#pragma once
class CImGuiManager : public CSingleton<CImGuiManager>
{
	SINGLETON(CImGuiManager)
public:
	void Init();
};
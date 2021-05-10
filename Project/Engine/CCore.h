#pragma once
class CCore : public CSingleton<CCore>
{
	SINGLETON(CCore)
private:
	HWND m_hWnd;
public:
	void Init(HWND _hWnd);
	void Progress();
};
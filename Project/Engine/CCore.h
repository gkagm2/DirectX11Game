#pragma once
class CCore : public CSingleton<CCore>
{
	SINGLETON(CCore)
private:
	HWND m_hWnd;

public:
	int Init(HWND _hWnd);
	void Progress();
	HWND GetWndHandle() { return m_hWnd; }
};
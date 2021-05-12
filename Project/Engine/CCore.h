#pragma once
class CCore : public CSingleton<CCore>
{
	SINGLETON(CCore)
private:
	HWND m_hWnd;				 // 윈도우 핸들
	Vector2 m_vWindowResolution; // 윈도우 해상도

public:
	int Init(HWND _hOutputWnd, const Vector2& _vWindowResolution, const Vector2& _vRenderResolution);
	void Progress();
	HWND GetWndHandle() { return m_hWnd; }
};
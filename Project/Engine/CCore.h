#pragma once
class CCore : public CSingleton<CCore>
{
	SINGLETON(CCore)
private:
	HWND m_hWnd;				 // 윈도우 핸들
	Vector2 m_vWindowResolution; // 윈도우 해상도

public:
	static HDC g_hDC;
	static HWND g_Wnd;

public:
	int Init(HWND _hOutputWnd, const Vector2& _vWindowResolution, const Vector2& _vRenderResolution);
	void Progress();
	HWND GetWndHandle() { return m_hWnd; }
	const Vector2& GetWindowResolution() const { return m_vWindowResolution; }
};
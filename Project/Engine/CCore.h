#pragma once
class CCore : public CSingleton<CCore>
{
	SINGLETON(CCore)
private:
	HWND m_hWnd;				 // ������ �ڵ�
	Vector2 m_vWindowResolution; // ������ �ػ�

	bool m_bOldVersionUpdate;	 // ���Ӱ� ������ �߰����� ��쿡�� ���

public:
	static HDC g_hDC;
	static HWND g_Wnd;


public:
	int Init(HWND _hOutputWnd, const Vector2& _vWindowResolution, const Vector2& _vRenderResolution);
	void Progress();
	HWND GetWndHandle() { return m_hWnd; }
	const Vector2& GetWindowResolution() const { return m_vWindowResolution; }
};
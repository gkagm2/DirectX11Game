#pragma once
class CCursor
{
private:
	static bool m_bVisible;
	static bool m_bCursorLock;
	static HCURSOR m_hCursor;
	static bool m_bCursorOnlyInScreen;

	static LONG m_tClientScreenRectL;
	static LONG m_tClientScreenRectR;
	static LONG m_tClientScreenRectT;
	static LONG m_tClientScreenRectB;

	static class CGameObject* m_pCursorObj;

public:
	static HDC m_dc;

private:
	static void _Init();
	static void _Update();

private:
	static void _UpdateCursorLock();
	static void _UpdateCursorInScreen();
public:

	static void SetVisible(bool _bVisible);
	static bool GetVisible() { return m_bVisible; }
	static void SetCursorLock(bool _bLock) { m_bCursorLock = _bLock; }
	static bool IsCursorLock() { return m_bCursorLock; }
	static void SetCursorInScreen(bool _bIsOnlyInScreen) { m_bCursorOnlyInScreen = _bIsOnlyInScreen; }

	// Offset : 왼쪽 상단이 중점인 상태에서의 오프셋
	static void SetCursor(SharedPtr<CTexture> _pTexture, const Vector2 _vOffset);

	static HCURSOR GetCursor() { return m_hCursor; }

public:
	friend class CCore;
};
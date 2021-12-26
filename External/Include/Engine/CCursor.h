#pragma once
class CCursor
{
private:
	static bool m_bVisible;
	static bool m_bCursorLock;
	static HCURSOR m_hCursor;
public:
	static HDC m_dc;

	static void Update();
public:

	static void SetVisible(bool _bVisible) { m_bVisible = _bVisible; }
	static bool GetVisible() { return m_bVisible; }
	static void SetCursorLock(bool _bLock) { m_bCursorLock = _bLock; }
	static bool IsCursorLock() { return m_bCursorLock; }

	// Offset : 왼쪽 상단이 중점인 상태에서의 오프셋
	static void SetCursor(SharedPtr<CTexture> _pTexture, const Vector2 _vOffset);

	static HCURSOR GetCursor() { return m_hCursor; }
	
};
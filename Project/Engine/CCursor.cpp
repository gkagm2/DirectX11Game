#include "pch.h"
#include "CCursor.h"
#include "dxgi.h"
#include "CTexture.h"
#include "CCore.h"
#include "CKeyManager.h"
#include "CObject.h"

bool CCursor::m_bVisible = true;
bool CCursor::m_bCursorLock = false;
bool CCursor::m_bCursorOnlyInScreen = false;

LONG CCursor::m_tClientScreenRectL =0;
LONG CCursor::m_tClientScreenRectR =0;
LONG CCursor::m_tClientScreenRectT =0;
LONG CCursor::m_tClientScreenRectB =0;
CGameObject* CCursor::m_pCursorObj = nullptr;

void CCursor::SetVisible(bool _bVisible)
{
    if (_bVisible && !m_bVisible) {
        ShowCursor(_bVisible);
        m_bVisible = _bVisible;
    }
    else if (!_bVisible && m_bVisible) {
        ShowCursor(_bVisible);
        m_bVisible = _bVisible;
    }
}


void CCursor::SetCursor(SharedPtr<CTexture> _pTexture, const Vector2 _vOffset)
{
}

void CCursor::_Init()
{
    m_bVisible = true;
    ShowCursor(m_bVisible);
    m_bCursorLock = false;
    m_bCursorOnlyInScreen = false;
}

void CCursor::_Update()
{
    HWND hWnd = GetFocus();
    if (nullptr == hWnd)
        return;

    // FIXED(Jang) : ������ ������ ���� �̺�Ʈ �߻� �� �����ϰ� ����°� ���ɻ� ���� ����.
    { // Client Screen ���� ���.
        static RECT rc2{};
        static POINT p1{}, p2{};

        GetClientRect(CCore::GetInstance()->GetWndHandle(), &rc2);

        // Ŭ���̾�Ʈ ũ�⸦ ��ǥ�� ��ȯ
        p1.x = rc2.left;
        p1.y = rc2.top;
        p2.x = rc2.right;
        p2.y = rc2.bottom;

        // Ŭ���̾�Ʈ ũ�⸦ ��ũ�� ũ��� ��ȯ
        ClientToScreen(CCore::GetInstance()->GetWndHandle(), &p1);
        ClientToScreen(CCore::GetInstance()->GetWndHandle(), &p2);

        m_tClientScreenRectL = p1.x;
        m_tClientScreenRectT = p1.y;
        m_tClientScreenRectR = p2.x;
        m_tClientScreenRectB = p2.y;
    }

    _UpdateCursorInScreen();
    _UpdateCursorLock();
}

void CCursor::_UpdateCursorLock()
{
    if (m_bCursorLock) {
        POINT middle{};
        middle.x = m_tClientScreenRectR - m_tClientScreenRectL;
        middle.y = m_tClientScreenRectB - m_tClientScreenRectT;
        static RECT rc{};
        rc.left = rc.right = middle.x;
        rc.top = rc.bottom = middle.y;
        ClipCursor(&rc);
    }
}

void CCursor::_UpdateCursorInScreen()
{
    if (m_bCursorOnlyInScreen) {
        RECT rt = { m_tClientScreenRectL, m_tClientScreenRectT,m_tClientScreenRectR, m_tClientScreenRectB };
        // �ش� ��ǥ�� �������� Ŀ���� ����
        ClipCursor(&rt);
    }
}
#include "pch.h"
#include "CKeyManager.h"
#include "CCore.h"

// TIP : VK_RETURN Ű�� ����Ű��
int g_arrVK[(int)E_Key::END] = {
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	VK_SPACE,
	'W',
	'S',
	'A',
	'D',
	'Q',
	'Z',
	'X',
	VK_CONTROL,
	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,
	VK_RETURN,
	VK_LSHIFT,
	VK_F4,
	VK_F5,
	VK_F8
};

CKeyManager::CKeyManager() :
	m_mousePosition{}
{

}
CKeyManager::~CKeyManager() {

}

void CKeyManager::Init()
{
	for (int i = 0; i < (int)E_Key::END; ++i)
		m_vecKey.push_back(tKey{ E_KeyState::NONE, false }); // state = none , press = false
}

void CKeyManager::Update()
{
	HWND hWnd = GetFocus();
	if (nullptr == hWnd) {
		for (UINT i = 0; i < m_vecKey.size(); ++i) {
			if (E_KeyState::PRESS == m_vecKey[i].eState || E_KeyState::HOLD == m_vecKey[i].eState)
				m_vecKey[i].eState = E_KeyState::RELEASE;
			else if (E_KeyState::RELEASE == m_vecKey[i].eState)
				m_vecKey[i].eState = E_KeyState::NONE;
			m_vecKey[i].bPress = false;
		}
	}
	else {
		for (UINT i = 0; i < m_vecKey.size(); ++i) {
			tKey& keyInfo = m_vecKey[i];
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) { // Ű�� ����
				if (!keyInfo.bPress) { // ������ ���� ���¸�
					keyInfo.bPress = true;
					keyInfo.eState = E_KeyState::PRESS;
				}
				else // �������� ����������
					keyInfo.eState = E_KeyState::HOLD;
			}
			else { // Ű�� ������ �ʾ�����
				if (keyInfo.bPress) { // �����־����� (���� ����)
					keyInfo.bPress = false;
					keyInfo.eState = E_KeyState::RELEASE;
				}
				else // �������� �ȴ����ִٸ�
					keyInfo.eState = E_KeyState::NONE;
			}
			m_vecKey[i] = keyInfo;
		}
		POINT ptMousePoint;
		GetCursorPos(&ptMousePoint);
		ScreenToClient(CCore::GetInstance()->GetWndHandle(), &ptMousePoint);
		m_mousePosition.x = (float)ptMousePoint.x;
		m_mousePosition.y = (float)ptMousePoint.y;
	}
}
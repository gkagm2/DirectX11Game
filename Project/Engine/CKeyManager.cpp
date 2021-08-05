#include "pch.h"
#include "CKeyManager.h"
#include "CCore.h"

// TIP : VK_RETURN 키가 엔터키임
int g_arrVK[(int)E_Key::END] = {
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	VK_SPACE,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',
	0x30, // number
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	VK_CONTROL, // Left Ctrl
	VK_MENU, // Left Alt
	VK_LSHIFT,
	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,
	VK_RETURN,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,
};

CKeyManager::CKeyManager() :
	m_vMousePosition{}
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
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) { // 키가 눌림
				if (!keyInfo.bPress) { // 누르지 않은 상태면
					keyInfo.bPress = true;
					keyInfo.eState = E_KeyState::PRESS;
				}
				else // 이전에도 눌려있으면
					keyInfo.eState = E_KeyState::HOLD;
			}
			else { // 키를 누르지 않았으면
				if (keyInfo.bPress) { // 눌려있었으면 (떼는 순간)
					keyInfo.bPress = false;
					keyInfo.eState = E_KeyState::RELEASE;
				}
				else // 이전에도 안눌려있다면
					keyInfo.eState = E_KeyState::NONE;
			}
			m_vecKey[i] = keyInfo;
		}
		POINT ptMousePoint;
		GetCursorPos(&ptMousePoint);
		ScreenToClient(CCore::GetInstance()->GetWndHandle(), &ptMousePoint);
		m_vMousePosition.x = (float)ptMousePoint.x;
		m_vMousePosition.y = (float)ptMousePoint.y;
	}


	//m_vMouseScrollDelta.y = 

}
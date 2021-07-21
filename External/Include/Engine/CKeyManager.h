#pragma once

// ���� �����ϴ� Ű
enum class E_Key {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE,
	WheelUp,
	WheelDown,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,	
	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,
	Ctrl,
	LBUTTON,
	RBUTTON,
	MBUTTON,
	Enter,
	LSHIFT,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	END,
};

enum class E_KeyState {
	NONE,		// ���� �����Ӱ� ���� �����ӿ��� ������ ���� ���� ��	 (�ƹ��͵� ������ ���� �� )
	PRESS,		// ���� �����ӿ��� �ȴ����� ���� �����ӿ� ���� ��		 (���� Ű�� ���� ��)
	HOLD,		// ���� �����Ӱ� ���� �����ӿ��� ������ ���� ��			 (Ű�� ������ �ִ� ���� ��)
	RELEASE,	// ���� �����ӿ��� �����ְ� ���� �����ӿ� �ȴ������� ��  (������ Ű�� �� ��)
};

struct tKey {
	E_KeyState eState;	// ������ Ű ����
	bool bPress;		// ���� �����ӿ� Ű�� �������� true
};

class CKeyManager : public CSingleton<CKeyManager>
{
	SINGLETON(CKeyManager)
private:
	vector<tKey> m_vecKey; // ���� �� �ִ� Ű�� ������ ��Ƴ���.
	Vector2 m_mousePosition;
	
public:
	void Init();
	void Update();

public:
	E_KeyState GetKeyState(E_Key _eKey) const { return m_vecKey[(int)_eKey].eState; }
	Vector2& GetMousePosition() { return m_mousePosition; }
};
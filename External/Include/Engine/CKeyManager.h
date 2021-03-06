#pragma once
// 현재 지원하는 키
enum class E_Key {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE,
	ESCAPE,
	TAB,
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

	NUMPAD0,
	NUMPAD1,
	NUMPAD2,
	NUMPAD3,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD7,
	NUMPAD8,
	NUMPAD9,

	LCtrl,
	LAlt,
	LShift,
	LBUTTON,
	RBUTTON,
	MBUTTON,
	Enter,
	
	Insert,
	Delete,
	PgUp,
	PgDn,
	Home,

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
	NONE,		// 이전 프레임과 현재 프레임에도 누르고 있지 않을 때	 (아무것도 누르지 않을 때 )
	PRESS,		// 이전 프레임에는 안눌리고 현재 프레임에 누를 때		 (현재 키를 누를 때)
	HOLD,		// 이전 프레임과 현재 프레임에도 누르고 있을 때			 (키를 누르고 있는 중일 때)
	RELEASE,	// 이전 프레임에는 눌려있고 현재 프레임에 안눌려있을 때  (눌렀던 키를 뗄 때)
};

struct tKey {
	E_KeyState eState;	// 현재의 키 상태
	bool bPress;		// 이전 프레임에 키가 눌렸으면 true
};

class CKeyManager : public CSingleton<CKeyManager>
{
	SINGLETON(CKeyManager)
private:
	vector<tKey> m_vecKey; // 누를 수 있는 키의 정보를 모아놓음.
	Vector2 m_vMousePosition;
	
public:
	void Init();
	void Update();

public:
	E_KeyState GetKeyState(E_Key _eKey) const { return m_vecKey[(int)_eKey].eState; }
	Vector2& GetMousePosition() { return m_vMousePosition; }
};
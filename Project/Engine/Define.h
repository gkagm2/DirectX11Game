#pragma once

#define SINGLETON(type)	private:\
						type();\
						virtual ~type();\
						friend class CSingleton<type>;

#define DeltaTime CTimeManager::GetInstance()->GetDeltaTime()

#define InputKeyHold(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::HOLD)
#define InputKeyPress(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::PRESS)
#define InputKeyRelease(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::RELEASE)

#define CLONE(type) type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) type* Clone() { assert(nullptr); return nullptr; }

#define MousePosition CKeyManager::GetInstance()->GetMousePosition()

#define DEVICE CDevice::GetInstance()->GetDevice()
#define CONTEXT CDevice::GetInstance()->GetDeviceContext()

// Math
#define PI 3.14159265f


enum class E_ResourceType {
	MATRIAL,
	SHADER,
	MESH,
	TEXTURE,
	SOUND,
	END,
};
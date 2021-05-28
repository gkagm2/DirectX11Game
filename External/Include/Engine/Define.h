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

enum class E_ConstBuffer {
	transform,
	end,
};

enum class E_ShaderStage {
	vertex		= 0x01, // 1
	hull		= 0x02, // 2
	domain		= 0x04, // 4
	geometry	= 0x08, // 8
	pixel		= 0x10, // 16
	compute		= 0x20, // 32
	all = vertex | hull | domain | geometry | pixel | compute,
};

enum class E_ComponentType {
	transform,
	meshRenderer,
	collider2d,
	collider3d,
	rigidbody2d,
	rigidbody3d,
	animator2d,
	animator3d,
	light2d,
	light3d,
	camera,
	particle,
	listener,
	audio,
	script,
	end,
};
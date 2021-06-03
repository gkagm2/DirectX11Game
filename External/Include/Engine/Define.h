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
	Vertex		= 0x01, // 1
	Hull		= 0x02, // 2
	Domain		= 0x04, // 4
	Geometry	= 0x08, // 8
	Pixel		= 0x10, // 16
	Compute		= 0x20, // 32
	All = Vertex | Hull | Domain | Geometry | Pixel | Compute,
};

enum class E_ComponentType {
	Transform,
	MeshRenderer,
	Collider2d,
	Collider3d,
	Rigidbody2d,
	Rigidbody3d,
	Animator2d,
	Animator3d,
	Light2d,
	Light3d,
	Camera,
	Particle,
	Listener,
	Audio,
	Script,
	End,
};

enum class E_Layer {
	Default,
	UI,
	End = 32,
};

enum class E_ProjectionType {
	Perspective,
	Orthographic,
};
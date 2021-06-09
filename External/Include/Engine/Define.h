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


#pragma region Components Define
				// ---------- In GameObject class ----------
#define GET_COMPONENT(name) C##name* name() { return (C##name*)m_arrComponent[(UINT)E_ComponentType::name];} 

#define GET_COMPONENT_COMPOTABLE \
	GET_COMPONENT(Transform)\
	GET_COMPONENT(MeshRenderer)\
	GET_COMPONENT(Camera)

// ---------- In Component class ----------
#define GET_OTHER_COMPONENT(name) C##name* name() { return GetGameObject()->name();}

#define GET_COMPONENT_FROM_GAMEOBJECT \
	GET_OTHER_COMPONENT(Transform)\
	GET_OTHER_COMPONENT(MeshRenderer)\
	GET_OTHER_COMPONENT(Camera)  
#pragma endregion



enum class E_ResourceType {
	Material,
	Shader,
	Mesh,
	Texture,
	Sound,
	End,
};

enum class E_ConstBuffer {
	Transform,		 // b0
	Material_Param,	 // b1
	End,
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

enum class E_RasterizerState {
	CullBack,
	CullFront,
	CullNone,  //�ո� �޸� �� �ø� ���Ѵ�.
	Wireframe, 
	End,
};

enum class E_BlendState {
	Defailt,
	AlphaBlend,
	AlphaBlend_Coverage,
	One_One,
	End,
};

enum class E_ShaderParam {
	Int_0,
	Int_1,
	Int_2,
	Int_3,

	Float_0,
	Float_1,
	Float_2,
	Float_3,

	Vector2_0,
	Vector2_1,
	Vector2_2,
	Vector2_3,

	Vector4_0,
	Vector4_1,
	Vector4_2,
	Vector4_3,

	Matrix_0,
	Matrix_1,
	Matrix_2,
	Matrix_3,

	Texture_0,
	Texture_1,
	Texture_2,
	Texture_3,
	Texture_4,
	Texture_5,
	Texture_6,
	Texture_7,
	TextureArr_0,
	TextureArr_1,
	TextureArr_2,
	TextureCube_0,
	TextureCube_1,
	TextureCube_2,
	Texture_End,
};

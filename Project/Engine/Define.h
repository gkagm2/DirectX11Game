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
#define CLONE_DISABLE(type) type* Clone() { assert(nullptr); return nullptr; } \
	type(const type& _origin) = delete;

#define MousePosition CKeyManager::GetInstance()->GetMousePosition()

#define DEVICE CDevice::GetInstance()->GetDevice()
#define CONTEXT CDevice::GetInstance()->GetDeviceContext()

#define FIND_GameObject_Layer(objName, layer) CSceneManager::GetInstance()->GetCurScene()->FindGameObject(objName, layer)
#define FIND_GameObject(objName) CSceneManager::GetInstance()->GetCurScene()->FindGameObject(objName)

// Math
#define PI 3.14159265f


#pragma region Components Define
				// ---------- In GameObject class ----------
#define GET_COMPONENT(name) C##name* name() { return (C##name*)m_arrComponent[(UINT)E_ComponentType::name];} 

#define GET_COMPONENT_COMPOTABLE \
	GET_COMPONENT(Transform)\
	GET_COMPONENT(MeshRenderer)\
	GET_COMPONENT(Camera)\
	GET_COMPONENT(Collider2D)\
	GET_COMPONENT(Collider3D)\
	GET_COMPONENT(Animator2D)\
	GET_COMPONENT(Light2D)\
	GET_COMPONENT(TileMap)\
	GET_COMPONENT(ParticleSystem)

// ---------- In Component class ----------
#define GET_OTHER_COMPONENT(name) C##name* name() { return GetGameObject()->name();}

#define GET_COMPONENT_FROM_GAMEOBJECT \
	GET_OTHER_COMPONENT(Transform)\
	GET_OTHER_COMPONENT(MeshRenderer)\
	GET_OTHER_COMPONENT(Camera)\
	GET_OTHER_COMPONENT(Collider2D)\
	GET_OTHER_COMPONENT(Collider3D)\
	GET_OTHER_COMPONENT(Animator2D)\
	GET_OTHER_COMPONENT(Light2D)\
	GET_OTHER_COMPONENT(TileMap)\
	GET_OTHER_COMPONENT(ParticleSystem)

#pragma endregion


// GPU RegisterNumber
#define REGISTER_NUM_Light2DBuffer 51
#define REGISTER_NUM_TileMapBuffer 52
#define REGISTER_NUM_NoiseTexture  53

#define REGISTER_NUM_Particle 12

// 삭제 우선순위로 설정
enum class E_ResourceType {
	Prefab,
	Material,
	GraphicsShader,
	ComputeShader,
	Mesh,
	Texture,
	Sound,
	End,
};

enum class E_ConstBuffer {
	Transform,		 // b0
	Material_Param,	 // b1
	Animation2D_Data,// b2
	Global,			 // b3
	End,
};

enum class E_ShaderStage {
	Vertex		= 0x01, // 1
	Hull		= 0x02, // 2
	Domain		= 0x04, // 4
	Geometry	= 0x08, // 8
	Pixel		= 0x10, // 16

	Compute		= 0x20, // 32

	NonePixel	= Vertex | Hull | Domain | Geometry,
	VertexPixel = Vertex | Pixel,
	Graphics	= Vertex | Hull | Domain | Geometry | Pixel,
	All = Vertex | Hull | Domain | Geometry | Pixel | Compute,
};


enum class E_ComponentType {
	Transform,
	Collider2D,
	Collider3D,
	Rigidbody2D,
	Rigidbody3D,
	Animator2D,
	Animator3D,
	Light2D,
	Light3D,
	Camera,

#pragma region 오직 하나만 렌더링할 수 있는 종류의 컴포넌트들. (오브젝트에 아래의 컴포넌트가 한개라도 존재할경우 아래의 다른 컴포넌트는 추가할 수 없음)
	MeshRenderer,
	ParticleSystem,
	TileMap,
#pragma endregion


	Listener,
	Audio,
	Script,
	End,
};
#define ONLY_ONE_POSSIBLE_RENDERING_START_IDX (UINT)E_ComponentType::MeshRenderer
#define ONLY_ONE_POSSIBLE_RENDERING_END_IDX (UINT)E_ComponentType::TileMap

#define MAX_SIZE_LAYER 32

// TODO (Jang) : 환경설정 파일을 따로 만들어볼까
#define _SHOOTING_GAME
#if defined(_SHOOTING_GAME)
enum class E_Layer {
	Default,
	Manager,
	Player,
	Enemy,
	Bullet,
	UI,
	End = MAX_SIZE_LAYER,
};
#elif defined(_SHOOTING_GAME)
enum class E_Layer {
	Default,
	Manager,
	UI,
	End = MAX_SIZE_LAYER,
};
#endif

enum class E_ProjectionType {
	Perspective,
	Orthographic,
};

enum class E_RasterizerState {
	CullBack,
	CullFront,
	CullNone,  //앞면 뒷면 다 컬링 안한다.
	Wireframe, 
	End,
};

enum class E_BlendState {
	Default,
	AlphaBlend,
	AlphaBlend_Coverage,
	One_One,
	End,
};

enum class E_DepthStencilState {
	Less, // default
	Less_Equal,
	Greater,
	No_Test,
	No_Write, // 테스트하되 깊이값 저장 X
	No_Test_No_Write, // 깊이 판정 검사 X 기록 X
	End
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
	TextureCube_0,
	TextureCube_1,
	Texture_End,
};

enum class E_LightType {
	Direction,
	Point,
	Spot
};
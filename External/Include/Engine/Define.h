#pragma once

#define SINGLETON(type)	private:\
						type();\
						virtual ~type();\
						friend class CSingleton<type>;

// 20210720 ImGui에 DeltaTime이 존해함. DeltaTime -> DT로 변환
#define DT CTimeManager::GetInstance()->GetDeltaTime()
#define InputKeyHold(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::HOLD)
#define InputKeyPress(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::PRESS)
#define InputKeyRelease(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::RELEASE)
#define MouseScrollDelta CMouseManager::GetInstance()->GetMouseScrollDelta()

#define CLONE(type) virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) type* Clone() { assert(nullptr); return nullptr; } \
	type(const type& _origin) = delete;

#define MousePosition CKeyManager::GetInstance()->GetMousePosition()

#define DEVICE CDevice::GetInstance()->GetDevice()
#define CONTEXT CDevice::GetInstance()->GetDeviceContext()

#define FIND_GameObject_Layer(objName, layer) CSceneManager::GetInstance()->GetCurScene()->FindGameObject(objName, layer)
#define FIND_GameObject(objName) CSceneManager::GetInstance()->GetCurScene()->FindGameObject(objName)

// Return UINT
#define COLOR_RGBA(r,g,b,a) (UINT)(((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

// Return BYTE
#define COLOR_R_FROM_RGBA(iColor) ((BYTE)iColor)
#define COLOR_G_FROM_RGBA(iColor) ((BYTE)(iColor >> 8))
#define COLOR_B_FROM_RGBA(iColor) ((BYTE)(iColor >> 16))
#define COLOR_A_FROM_RGBA(iColor) ((BYTE)(iColor >> 24))


// Math
#define PI 3.14159265f

#define FLOAT_MIN (std::numeric_limits<float>::min)()
#define FLOAT_MAX (std::numeric_limits<float>::max)()

// Make String
//#define stringfiy(name) # name // FIXED (Jang) : tchar 타입은 어떻게하지

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
	GET_COMPONENT(ParticleSystem)\
	GET_COMPONENT(Rigidbody2D)\
	GET_COMPONENT(AudioSource)\
	GET_COMPONENT(RectTransform)\
	GET_COMPONENT(SpriteRenderer)\
	GET_COMPONENT(CanvasRenderer)\
	GET_COMPONENT(TextUI)\
	GET_COMPONENT(ImageUI)\
	GET_COMPONENT(ButtonUI)

// TIP : 추가 할 경우 
// CGameObject .cpp,.h에 선언하기. 
// .cpp에서 CreateComponent 함수에서도 추가하기.
// external에서 ComponentTypeToStr 함수에서도 추가히기
// ComponentType도 설정
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
	GET_OTHER_COMPONENT(ParticleSystem)\
	GET_OTHER_COMPONENT(Rigidbody2D)\
	GET_OTHER_COMPONENT(AudioSource)\
	GET_OTHER_COMPONENT(RectTransform)\
	GET_OTHER_COMPONENT(SpriteRenderer)\
	GET_OTHER_COMPONENT(CanvasRenderer)\
	GET_OTHER_COMPONENT(TextUI)\
	GET_OTHER_COMPONENT(ImageUI)\
	GET_OTHER_COMPONENT(ButtonUI)
#pragma endregion





// GPU RegisterNumber
#define REGISTER_NUM_Light2DBuffer 51
#define REGISTER_NUM_TileMapBuffer 52
#define REGISTER_NUM_NoiseTexture  53

#define REGISTER_NUM_Particle 12


enum class E_SceneMode {
	Play,
	Pause,
	Stop,
};

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
// Warning : Resource Type 추가 시 Extern.cpp에서 ResourceTypeToStr함수에 문자열 추가하기


enum class E_ConstBuffer {
	Transform,		 // b0
	Material_Param,	 // b1
	Animation2D_Data,// b2
	Global,			 // b3
	RectTransform,	 // b4
	Particle,		 // b5
	End,
};

// Pipeline Stage
enum class E_ShaderStage {
	Vertex = 0x01, // 1
	Hull = 0x02, // 2
	Domain = 0x04, // 4
	Geometry = 0x08, // 8
	Pixel = 0x10, // 16

	Compute = 0x20, // 32

	NonePixel = Vertex | Hull | Domain | Geometry,
	VertexPixel = Vertex | Pixel,
	Graphics = Vertex | Hull | Domain | Geometry | Pixel,
	All = Vertex | Hull | Domain | Geometry | Pixel | Compute,
};

enum class E_RenderTimePoint {
	Forward,
	Particle,
	PostEffect,
	Collider,
	Canvas,
	None,
};

enum class E_ComponentType {
	Transform,
	BoundingBox,
	Collider2D,
	Collider3D,
	Rigidbody2D,
	Rigidbody3D,
	Animator2D,
	Animator3D,
	Light2D,
	Light3D,
	Camera,
	RectTransform,
	// TODO (Jang) : Terrain, 추가
	// TODO (Jang) : SkyBox, 추가
#pragma region 오직 하나만 렌더링할 수 있는 종류의 컴포넌트들. (오브젝트에 아래의 컴포넌트가 한개라도 존재할경우 아래의 다른 컴포넌트는 추가할 수 없음)
	CanvasRenderer,
	MeshRenderer,
	SpriteRenderer,
	ParticleSystem,
	TileMap,
#pragma endregion

#pragma region UI Component
	//CanvasUI,
	ButtonUI,
	TextUI,
	ImageUI,
#pragma endregion

	Listener,
	AudioSource,
	End,

	Script,
};
// Warning extern.cpp에 ComponentTypeToStr 함수에도 추가해주기

#define ONLY_ONE_POSSIBLE_RENDERING_START_IDX (UINT)E_ComponentType::CanvasRenderer
#define ONLY_ONE_POSSIBLE_RENDERING_END_IDX (UINT)E_ComponentType::TileMap


#define MAX_SIZE_LAYER 32
#define MAX_SIZE_TAG 32

// TODO (Jang) : 환경설정 파일을 따로 만들어볼까

#define NUM_LAYER_TILEMAP 29
#define NUM_LAYER_UI 30
#define NUM_LAYER_CLIENT 31

//#define _SHOOTING2D_GAME
//#define _CAPTAIN_FOREVER_GAME
#define _BUTCHER_GAME

#ifdef _SHOOTING2D_GAME
enum class E_Layer {
	Player,
	Bullet,
	Enemy,
	Manager,

	End = MAX_SIZE_LAYER,
};
#endif
#ifdef _CAPTAIN_FOREVER_GAME
//enum class E_Layer {
//	Default,
//#if defined(_SHOOTING_GAME)
//	Manager, // 1
//	Player, // 2
//	Enemy, // 3
//	Bullet,// 4
//#elif defined(_SHOOTING_GAME)
//	Manager,// 1
//#elif defined(_CAPTAIN_FOREVER_GAME)
//	Module,//1
//	Manager,//2
//#endif
//	UI,//31
//	End = MAX_SIZE_LAYER,
//};
#endif
#ifdef _BUTCHER_GAME
enum class E_Layer {
	Default,
	Character,
	BackgroundMap,
	TileMap = NUM_LAYER_TILEMAP,
	UI = NUM_LAYER_UI,
	CLIENT = NUM_LAYER_CLIENT,
	End = MAX_SIZE_LAYER,
};

enum class E_Tag {
	Player,
	Bullet,
	Enemy,
	Item,
};
tstring TagToString(E_Tag _eTag);

#endif




enum class E_ProjectionType {
	Perspective,
	Orthographic,
};
#define PROJECTION_TYPE_COUNT 2

enum class E_RasterizerState {
	CullBack,
	CullFront,
	CullNone,  //앞면 뒷면 다 컬링 안한다. (다 보여줌)
	Wireframe,
	End,
};

enum class E_BlendState {
	Default,
	AlphaBlend,
	AlphaBlend_Coverage, // 깊이 정렬 안해도 알아서 alpha처리
	One_One, // 1대1로 섞음
	End,
};

enum class E_DepthStencilState {
	Less, // default
	Less_Equal,
	Greater,
	No_Test,
	No_Write, // 깊이 판정 검사는 하되, DSTex에 깊이값 저장 X
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
	Spot,
	End, // 사용하지 않음
};
tstring LightTypeToStr(E_LightType _eType);


// TIP : 추가 시 particle_update.fx 파일에 조건문 추가하기
enum class E_ParticleShape {
	Circle_Spread,
	Circle_Combine,
	Cone_Spread,
	Edge,
	Sphere, // 3d
	End, // 사용하지 않음
};
tstring ParticleShapeToStr(E_ParticleShape _eType);
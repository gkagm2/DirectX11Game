#pragma once

#define SINGLETON(type)	private:\
						type();\
						virtual ~type();\
						friend class CSingleton<type>;


#define SINGLETON_SCRIPT(type)	private:\
									static type* m_pInst;\
								public:\
									type* GetInstance() { \
								if (!m_pInst)\
									m_pInst = this;\
								return m_pInst;\
								}\
								type();\
								virtual ~type();
#define SINGLETON_SCRIPT_CPP(type) type* type::m_pInst = nullptr;

#define SAFE_DELETE(value) if(value) {\
		delete value;\
		value = nullptr;\
	}
#define SAFE_DELETE_PTR(value) if(nullptr != value.Get()) { \
		delete value.Get();\
		value = nullptr;\
	}

// ��������ڿ��� Param�� �߰��� ��� ������Ѿ� ��
#define AddParam_LINKOBJ(pTargetObj_OUT, pOriginTargetObj, TYPE) if (((TYPE*)&_origin)->GetGameObject()->IsExistGameObjectInTree(pOriginTargetObj))\
CObject::LinkObjectWhenCloneGameObjEvn(this, &pTargetObj_OUT, pOriginTargetObj);

// 20210720 ImGui�� DeltaTime�� ������. DeltaTime -> DT�� ��ȯ
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

#define FIND_GameObject_Layer(objName, layer) dynamic_cast<CGameObject*>(CSceneManager::GetInstance()->GetCurScene()->FindGameObject(objName, layer))
#define FIND_GameObject(objName) dynamic_cast<CGameObject*>(CSceneManager::GetInstance()->GetCurScene()->FindGameObject(objName))

// OUT : ���� ������ ����, COMPONENT_TYPE : ã�� ������Ʈ Ÿ��
#define FIND_Component(OUT, COMPONENT_TYPE) { CScene* __pCurScene = CSceneManager::GetInstance()->GetCurScene(); \
	vector<CGameObject*> __root; \
	__pCurScene->GetRootGameObjects(__root); \
	for (size_t i = 0; i < __root.size(); ++i) {\
			OUT = __root[i]->GetComponent<COMPONENT_TYPE>(); \
			if (OUT) break; \
				OUT = __root[i]->FindComponentInChilds<COMPONENT_TYPE>(); \
				if (OUT) break; \
	}}

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


#define DEFAULT_UI_CAM_POS Vector3(9999.f,9999.f, -9999.f)

// Make String
//#define stringfiy(name) # name // FIXED (Jang) : tchar Ÿ���� �������

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
	GET_COMPONENT(Light3D)\
	GET_COMPONENT(TileMap)\
	GET_COMPONENT(ParticleSystem)\
	GET_COMPONENT(Rigidbody2D)\
	GET_COMPONENT(AudioSource)\
	GET_COMPONENT(RectTransform)\
	GET_COMPONENT(SpriteRenderer)\
	GET_COMPONENT(CanvasRenderer)\
	GET_COMPONENT(TextUI)\
	GET_COMPONENT(ImageUI)\
	GET_COMPONENT(ButtonUI)\
	GET_COMPONENT(Skybox)

// TIP : �߰� �� ��� 
// CGameObject .cpp,.h�� �����ϱ�. 
// .cpp���� CreateComponent �Լ������� �߰��ϱ�.
// external���� ComponentTypeToStr �Լ������� �߰�����
// ComponentType�� ����
// Render Component�� ���
//	1. CGameObject.cpp - Render()�� �߰�
//	2. CCamera.cpp - SortObject�� ���� ���� �߰�
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
	GET_OTHER_COMPONENT(Light3D)\
	GET_OTHER_COMPONENT(TileMap)\
	GET_OTHER_COMPONENT(ParticleSystem)\
	GET_OTHER_COMPONENT(Rigidbody2D)\
	GET_OTHER_COMPONENT(AudioSource)\
	GET_OTHER_COMPONENT(RectTransform)\
	GET_OTHER_COMPONENT(SpriteRenderer)\
	GET_OTHER_COMPONENT(CanvasRenderer)\
	GET_OTHER_COMPONENT(TextUI)\
	GET_OTHER_COMPONENT(ImageUI)\
	GET_OTHER_COMPONENT(ButtonUI)\
	GET_OTHER_COMPONENT(Skybox)
#pragma endregion





// GPU RegisterNumber
#define REGISTER_NUM_Light2DBuffer 51
#define REGISTER_NUM_TileMapBuffer 52
#define REGISTER_NUM_NoiseTexture  53
#define REGISTER_NUM_Light3DBuffer 54

#define REGISTER_NUM_Particle 12


enum class E_SceneMode {
	Play,
	Pause,
	Stop,
};

// ���� �켱������ ����
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
// Warning : Resource Type �߰� �� Extern.cpp���� ResourceTypeToStr�Լ��� ���ڿ� �߰��ϱ�


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
	ShadowDepth,
	Deferred,
	Light,
	Particle,
	PostEffect,
	Collider,
	Canvas,
	After,
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
	//Terrain,
	//Decal,
	// TODO (Jang) : Terrain, �߰�
#pragma region ���� �ϳ��� �������� �� �ִ� ������ ������Ʈ��. (������Ʈ�� �Ʒ��� ������Ʈ�� �Ѱ��� �����Ұ�� �Ʒ��� �ٸ� ������Ʈ�� �߰��� �� ����)
	CanvasRenderer,
	MeshRenderer,
	SpriteRenderer,
	ParticleSystem,
	Skybox,
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

enum class E_ComponentTypeOld {
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

#pragma region ���� �ϳ��� �������� �� �ִ� ������ ������Ʈ��. (������Ʈ�� �Ʒ��� ������Ʈ�� �Ѱ��� �����Ұ�� �Ʒ��� �ٸ� ������Ʈ�� �߰��� �� ����)
	CanvasRenderer,
	MeshRenderer,
	SpriteRenderer,
	ParticleSystem,
	Skybox,
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
// Warning extern.cpp�� ComponentTypeToStr �Լ����� �߰����ֱ�
// CGameObject.cpp���� �߰�.
// Render Component�� ��� CCamera.cpp������ �߰�.

#define ONLY_ONE_POSSIBLE_RENDERING_START_IDX (UINT)E_ComponentType::CanvasRenderer
#define ONLY_ONE_POSSIBLE_RENDERING_END_IDX (UINT)E_ComponentType::TileMap


#define MAX_SIZE_LAYER 32
#define MAX_SIZE_TAG 32

// TODO (Jang) : ȯ�漳�� ������ ���� ������

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
	Object,
	BackgroundMap,
	WayPoint_Tool,
	Wall,
	ObjectParticle,

	TileMap = NUM_LAYER_TILEMAP,
	UI = NUM_LAYER_UI,
	CLIENT = NUM_LAYER_CLIENT,
	End = MAX_SIZE_LAYER,
};
tstring LayerToString(E_Layer _eLayer);

enum class E_Tag {
	Player, // 0 
	Bullet,	// 1
	Enemy,	// 2
	Object, // 3 Interaction Object
	Item,	// 4�÷��̾ ���� �� �ִ� ������
	DangerObj,//6 ������ ������ ������Ʈ
	Player_Bullet, // 7
	Enemy_Bullet, // 8
	Player_GroundCheck,//9
	Enemy_GroundCheck,//10
	ObjParticle, //11
	Trigger,
	End
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
	CullNone,  //�ո� �޸� �� �ø� ���Ѵ�. (�� ������)
	Wireframe,
	End,
};

enum class E_BlendState {
	Default,
	AlphaBlend,
	AlphaBlend_Coverage, // ���� ���� ���ص� �˾Ƽ� alphaó��
	One_One, // 1��1�� ����
	End,
};

enum class E_DepthStencilState {
	Less, // default
	Less_Equal,
	Greater,
	No_Test,
	No_Write, // ���� ���� �˻�� �ϵ�, DSTex�� ���̰� ���� X
	No_Test_No_Write, // ���� ���� �˻� X ��� X
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
	End, // ������� ����
};
tstring LightTypeToStr(E_LightType _eType);


// TIP : �߰� �� particle_update.fx ���Ͽ� ���ǹ� �߰��ϱ�
enum class E_ParticleShape {
	Circle_Spread,
	Circle_Combine,
	Cone_Spread,
	Edge,
	Sphere, // 3d
	End, // ������� ����
};
tstring ParticleShapeToStr(E_ParticleShape _eType);

//#define OLD_VERSION

#include "pch.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CEventManager.h"
#include "CGraphicsShader.h"
#include "CSceneManager.h"
#include "CScene.h"

CResourceManager::CResourceManager() :
	m_bFixed(false)
{
}

CResourceManager::~CResourceManager()
{
	Safe_Delete_Vector(m_vecCloneMtrl);

	for (UINT i = 0; i < (UINT)E_ResourceType::End; ++i)
		Safe_Delete_UnorderedMap(m_umapResource[i]);
}

void CResourceManager::Init()
{
	CreateDefaultMesh();
	CreateDefaultCircle2DMesh();
	//CreateDefaultCubeMesh3D();
	CreateDefaultShader();
	CreateDefaultTexture();
	CreateComputeShader();
	CreateDefaultMaterial();
	InitSound(); // FMOD �ʱ�ȭ

	// Texture �ε�
	LoadResourcesFromDir<CTexture>(_T("texture\\"), _T("*.*"));

	// ������ �ε�
	LoadResourcesFromDir<CPrefab>(_T("prefab\\"), _T("*.pref"));

	// ���͸��� �ε�
	LoadResourcesFromDir<CMaterial>(_T("material\\"), _T("*.mtrl"));

}

void CResourceManager::CreateDefaultMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	////////////////
	// RectMesh ����

	// ���� �����
	VTX vertex;
	vertex.vPos = Vector3(-0.5f, 0.5f, 0.f);
	vertex.vColor = Vector4(0.0f, 0.0f, 0.50f, 1.0f);
	vertex.vUV = Vector2(0.f, 0.f);
	vecVtx.push_back(vertex);

	vertex.vPos = Vector3(0.5f, 0.5f, 0.f);
	vertex.vColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	vertex.vUV = Vector2(1.f, 0.f);
	vecVtx.push_back(vertex);

	vertex.vPos = Vector3(0.5f, -0.5f, 0.f);
	vertex.vColor = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
	vertex.vUV = Vector2(1.f, 1.f);
	vecVtx.push_back(vertex);

	vertex.vPos = Vector3(-0.5f, -0.5f, 0.f);
	vertex.vColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	vertex.vUV = Vector2(0.f, 1.f);
	vecVtx.push_back(vertex);

	// create index buffer
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2); 
	vecIdx.push_back(0); 
	vecIdx.push_back(2); 
	vecIdx.push_back(3);
	/*
	0-------1
	|��     |
	|  ��   |
	|    �� |
	3-------2
	*/

	CMesh* pMesh = new CMesh();
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_RectMesh, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);

	///////////////////
	// RectLineMesh ����
	/*
	0-------1
	|		|
	|		|
	|		|
	3-------2
	*/
	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_RectLineMesh, pMesh);

	///////////////////
	// Point Mesh ����
	vertex = {};
	vecVtx.clear();
	vecIdx.clear();

	vertex.vPos = Vector3(0.f, 0.f, 0.f);
	vertex.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
	vertex.vUV = Vector2(0.f, 0.f);

	vecVtx.push_back(vertex);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_PointMesh, pMesh);


	////////////////////
	// line strip Mesh ����
	vertex = {};
	vecVtx.clear();
	vecIdx.clear();

	vertex.vPos = Vector3(0.f, 0.f, 0.f);
	vertex.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
	vertex.vUV = Vector2(0.f, 0.f);

	vecVtx.push_back(vertex);
	vecIdx.push_back(0);
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX)* (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT)* (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_LineStripMesh, pMesh);
}

void CResourceManager::CreateDefaultCircle2DMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	// Circle Mesh, Circle Line Mesh �����

	// ���� ����
	VTX vtx = {};

	vtx.vPos = Vector3(0.f, 0.f, 0.f);
	vtx.vUV = Vector2(0.5f, 0.5f);
	Vector4 vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vtx.vColor = vColor;
	vecVtx.push_back(vtx);
	
	float fRadius = 0.5f;
	int iSidesCnt = 40; // ���� ����
	float fRadian = XM_2PI / (float)iSidesCnt;

	// �׵θ��� ���� ����
	for (int i = 0; i < iSidesCnt + 1; ++i) {
		vtx.vPos.x = cosf(fRadian * (float)i) * fRadius;
		vtx.vPos.y = sinf(fRadian * (float)i) * fRadius;
		vtx.vColor = vColor;
		vtx.vUV;
		
		vecVtx.push_back(vtx);
	}

	// �ε��� ����

	// Circle Mesh
	for (int i = 0; i < iSidesCnt; ++i) {
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	CMesh* pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
	AddRes(STR_KEY_CircleMesh, pMesh);

	// Circle Line Mesh 
	vecIdx.clear();
	for (int i = 0; i < iSidesCnt + 1; ++i)
		vecIdx.push_back(i + 1);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
	AddRes(STR_KEY_CircleLineMesh, pMesh);
}

void CResourceManager::CreateDefaultCubeMesh3D()
{
	// TODO (Jang): 20210606 UV ��ǥ �� �ֱ�
	// 
	// ���� �����
	VTX vertices[] = {
		{ Vector3(-1.0f, 1.0f, -1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, -1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f) },
	};

	// create index buffer
	//UINT arrIdx[6] = { 0 ,1, 3, 3, 1, 2 };
	/*
		3----- 2
		  ��
	0-------1  |
	|��     |  |
	|  ��   |  |
	|   7�� |--6
	4-------5
	*/

	UINT arrIdx[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	CMesh* pMesh = new CMesh();
	pMesh->Create(vertices, sizeof(VTX) * 8, arrIdx, sizeof(UINT) * 36, D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_CubeMesh, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);
}

void CResourceManager::CreateDefaultShader()
{
	// --------------------------
	// �⺻ ���̴� ���� (AlphaBlend Coveratge)
	CGraphicsShader* pShader = new CGraphicsShader(E_RenderPov::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// Shader Param
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	AddRes(STR_KEY_StdAlphaBlend_CoverageShader, pShader);

	// -----------------------
	// �⺻ ���̴� ���� (AlphaBlend)
	pShader = new CGraphicsShader(E_RenderPov::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend);
	// Shader Param
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	// Test Shader Param
	/*pShader->AddShaderParam({ E_ShaderParam::Int_0,	  _T("Test Param Int") });
	pShader->AddShaderParam({ E_ShaderParam::Float_0,   _T("Test Param Float") });
	pShader->AddShaderParam({ E_ShaderParam::Vector2_1, _T("Test Param Vec2") });
	pShader->AddShaderParam({ E_ShaderParam::Vector4_2, _T("Test Param Vec4") });
	pShader->AddShaderParam({ E_ShaderParam::Texture_0, _T("Output Texture1") });
	pShader->AddShaderParam({ E_ShaderParam::Texture_1, _T("Output Texture2") });*/

	AddRes(STR_KEY_StdAlphaBlendShader, pShader);

	//----------------------
	// Light2D ���̴� ����
	pShader = new CGraphicsShader(E_RenderPov::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderLight2D);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderLight2D);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	
	AddRes(STR_KEY_StdLight2DShader, pShader);

	//----------------------
	// LineStript ���̴� ����
	pShader = new CGraphicsShader(E_RenderPov::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderLineStrip);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderLineStrip);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Color") });

	AddRes(STR_KEY_StdLineStripShader, pShader);

	//----------------------
	// Collider2D ���̴� ����
	pShader = new CGraphicsShader(E_RenderPov::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderCollider2D);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderCollider2D);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Topology
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// OM (Output Merge)
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Default Rect Color") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_1, _T("Connect Rect Color") });

	AddRes(STR_KEY_Collider2DShader, pShader);

	//-----------------------
	// Ÿ�ϸ� ���̴� ����
	pShader = new CGraphicsShader(E_RenderPov::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderTileMap);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderTileMap);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_0, _T("x face count") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Int_1, _T("y face count") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector2_0, _T("Tile UV LeftTop size") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector2_1, _T("Tile UV Size") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Atlas Texture") });
	

	AddRes(STR_KEY_TileMapShader, pShader);

	//-------------------------
	// ��ƼŬ ���� ���̴� ����
	pShader = new CGraphicsShader(E_RenderPov::Particle);
	pShader->CreateVertexShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_VTX_Particle);
	pShader->CreateGeometryShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_GEO_Particle);
	pShader->CreatePixelShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_PIX_Particle);

	// Topology
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Blend
	pShader->SetBlendState(E_BlendState::AlphaBlend);

	// Depth Stencil
	pShader->SetDepthStencilState(E_DepthStencilState::No_Write);

	// ShaderParam
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Start Color") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_1, _T("End Color") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_2, _T("Start Scale") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_3, _T("End Scale") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Particle Texture") });

	AddRes(STR_KEY_ParticleShader, pShader);

	//-------------------------
	// Distortion(�ְ�) ���̴� ����
	pShader = new CGraphicsShader(E_RenderPov::PostEffect);
	pShader->CreateVertexShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_VTX_Distortion);
	pShader->CreatePixelShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_PIX_Distortion);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Depth Stencil
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	AddRes(STR_KEY_DistortionShader, pShader);

	//---------------------------
	// FishEye Shader
	pShader = new CGraphicsShader(E_RenderPov::PostEffect);
	pShader->CreateVertexShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_VTX_FishEye);
	pShader->CreatePixelShader(STR_FILE_PATH_PostEffectShader, STR_FUNC_NAME_PIX_FishEye);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Depth Stencil
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	AddRes(STR_KEY_FishEyeShader, pShader);

	//----------------------------
	// Canvas Shader
	pShader = new CGraphicsShader(E_RenderPov::Forward);
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXCanvasShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXCanvasShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	// Shader Param
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Texture_0, _T("Output Texture") });
	pShader->AddShaderParam(TShaderParam{ E_ShaderParam::Float_2, _T("Size") });
	AddRes(STR_KEY_CanvasShader, pShader);
}

void CResourceManager::CreateDefaultMaterial()
{
	// �⺻ ���� ���� (AlphaBlend Coverage)
	CMaterial* pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderAlphaBlendCV = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlend_CoverageShader);
	pMtrl->SetShader(pShaderAlphaBlendCV);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl, pMtrl);

	// �⺻ ���� ���� (AlphaBlend)
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderAlphaBlend = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlendShader);
	pMtrl->SetShader(pShaderAlphaBlend);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes<CMaterial>(STR_KEY_StdAlphaBlendMtrl, pMtrl);

	// Line Strip
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLineStrip = LoadRes<CGraphicsShader>(STR_KEY_StdLineStripShader);
	pMtrl->SetShader(pShaderLineStrip);
	pMtrl->SetData(E_ShaderParam::Vector4_0, Vector4{ 1.f,0.f,1.f,1.f });
	AddRes<CMaterial>(STR_KEY_LineStripMtrl, pMtrl);

	// Light2D ���� ����
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLight2D = LoadRes<CGraphicsShader>(STR_KEY_StdLight2DShader);
	pMtrl->SetShader(pShaderLight2D);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes<CMaterial>(STR_KEY_StdLight2DMtrl, pMtrl);

	// Collider2D ���� ����
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderCollider2D = LoadRes<CGraphicsShader>(STR_KEY_Collider2DShader);
	pMtrl->SetShader(pShaderCollider2D);
	pMtrl->SetData(E_ShaderParam::Vector4_0, Vector4(0.2f, 0.9f, 0.2, 1.f)); // green
	pMtrl->SetData(E_ShaderParam::Vector4_1, Vector4(0.9f, 0.2f, 0.2f, 1.f)); // red
	AddRes(STR_KEY_Collider2DMtrl, pMtrl);

	// Ÿ�ϸ� ���� ����
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderTileMap = LoadRes<CGraphicsShader>(STR_KEY_TileMapShader);
	pMtrl->SetShader(pShaderTileMap);
	AddRes(STR_KEY_TileMapMtrl, pMtrl);

	// ��ƼŬ ���� ����
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderParticle = LoadRes<CGraphicsShader>(STR_KEY_ParticleShader);
	pMtrl->SetShader(pShaderParticle);
	AddRes(STR_KEY_ParticleMtrl, pMtrl);

	// �ְ�(Distortion) ���� ����
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderDistortion = LoadRes<CGraphicsShader>(STR_KEY_DistortionShader);
	pMtrl->SetShader(pShaderDistortion);

	SharedPtr<CTexture> pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	AddRes(STR_KEY_DistortionMtrl, pMtrl);

	// FishEye ���� ����
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderFishEye = LoadRes<CGraphicsShader>(STR_KEY_FishEyeShader);
	pMtrl->SetShader(pShaderFishEye);

	pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	AddRes(STR_KEY_FishEyeMtrl, pMtrl);

	// Canvas ���� ����
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderCanvas = LoadRes<CGraphicsShader>(STR_KEY_CanvasShader);
	pMtrl->SetShader(pShaderCanvas);
	pMtrl->SetData(E_ShaderParam::Texture_0, LoadRes<CTexture>(STR_PATH_Box).Get());
	AddRes(STR_KEY_CanvasMtrl, pMtrl);
}


#include "CTestShader.h"
#include "CParticleUpdateShader.h"
void CResourceManager::CreateComputeShader()
{
	// TODO (Jang) : Test�� ��ǻƮ ���̴��� �����ϰ�����. ���߿� ��ġ��
	CComputeShader* pShader = new CTestShader;
	pShader->CreateComputeShader(STR_FILE_PATH_TestComputeShader, "CS_Test");
	AddRes(STR_KEY_TestComputeShader, pShader);

	/////////////////////////
	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(STR_FILE_PATH_ParticleUpdateShader, STR_FUNC_NAME_ParticleUpdate);
	AddRes(STR_KEY_ParticleUpdateShader, pShader);
}

void CResourceManager::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
		assert(nullptr);

	// 32�� ä�� ����
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResourceManager::CreateDefaultTexture()
{
	SharedPtr<CTexture> pTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_FILE_PATH_NoiseTexture1);
	CResourceManager::GetInstance()->LoadRes<CTexture>(STR_FILE_PATH_NoiseTexture2);

	pTexture->UpdateData(E_ShaderStage::All, REGISTER_NUM_NoiseTexture);

	g_globalConst.vNoiseResolution = Vector2((float)pTexture->GetDimension().x, (float)pTexture->GetDimension().y);
}

// param(_iBindFlag) : D3D11_BIND_FLAG
SharedPtr<CTexture> CResourceManager::CreateTexture(const tstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	assert(nullptr == FindRes<CTexture>(_strKey));

	CTexture* pTexture = new CTexture;
	pTexture->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);
	AddRes<CTexture>(_strKey, pTexture);
	m_bFixed = true;
	return pTexture;
}

SharedPtr<CTexture> CResourceManager::CreateTexture(const tstring& _strKey, ComPtr<ID3D11Texture2D> _pTexture2D)
{
	assert(nullptr == FindRes<CTexture>(_strKey));

	CTexture* pTexture = new CTexture;
	pTexture->Create(_pTexture2D);
	AddRes<CTexture>(_strKey, pTexture);
	m_bFixed = true;
	return pTexture;
}

void CResourceManager::ChangeResourceKeyEvn(const tstring& _strKey, const tstring& _strKeyChange, E_ResourceType _eResourceType)
{
	TEvent even = {};
	even.eType = E_EventType::Chagne_ResourceKey;
	tstring* pStrKey = new tstring(_strKey);
	tstring* pStrKeyChange = new tstring(_strKeyChange);
	even.lparam = (DWORD_PTR)pStrKey;
	even.wparam = (DWORD_PTR)pStrKeyChange;
	even.mparam= (DWORD_PTR)_eResourceType; 
	
	CEventManager::GetInstance()->AddEvent(even);
}

void CResourceManager::DeleteCopiedMaterialEvn(const tstring& _strKey)
{
	TEvent even = {};
	even.eType = E_EventType::Remove_Material;
	tstring* pStrKey = new tstring(_strKey);
	even.lparam = (DWORD_PTR)pStrKey;
	CEventManager::GetInstance()->AddEvent(even);
}

void CResourceManager::DeleteCustomResourceEvn(const tstring& _strKey, E_ResourceType _eResourceType)
{
	TEvent even = {};
	even.eType = E_EventType::Remove_Resource;
	tstring* pStrKey = new tstring(_strKey);
	even.lparam = (DWORD_PTR)pStrKey;
	even.wparam = (DWORD_PTR)_eResourceType;
	CEventManager::GetInstance()->AddEvent(even);
}

void CResourceManager::_DeleteCopiedMaterial(const tstring& _strKey)
{
	// ����� ���͸��󿡼� ã��.
	SharedPtr<CMaterial> pMtrl = FindRes<CMaterial>(_strKey);
	tstring strRelativePath = pMtrl->GetRelativePath();
	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath += strRelativePath;

	if (pMtrl.Get()) {
		{ // ����� ���͸���
			vector<CMaterial*>::iterator iter = m_vecCloneMtrl.begin();
			for (; iter != m_vecCloneMtrl.end(); ++iter) {
				if ((*iter)->GetKey() == _strKey) {
					m_bFixed = true;
					CMaterial* pMtrl = *iter;
					if (pMtrl)
						delete pMtrl;
					m_vecCloneMtrl.erase(iter);
					break;
				}
			}
		}
		{ // ���ҽ��� ���͸���
			unordered_map<tstring, CResource*>::iterator iter = m_umapResource[(UINT)E_ResourceType::Material].begin();
			for (; iter != m_umapResource[(UINT)E_ResourceType::Material].end(); ++iter) {
				if ((*iter).first == _strKey) {
					m_bFixed = true;
					CResource* pMtrl = iter->second;
					if (pMtrl)
						delete pMtrl;
					m_umapResource[(UINT)E_ResourceType::Material].erase(iter);
					break;
				}
			}
		}
	}

	if (-1 == _tremove(strPath.c_str())) {
		//assert(nullptr && _T("���� ���� ����"));
	}
}

/// <param name="_strOldKey">��θ� + Ű �̸� + Ȯ������� �Ǿ��ִ� Key</param>
/// <param name="_strNewKey">Ű �̸�</param>
/// <param name="_eResourceType">���ҽ� Ÿ��</param>
bool CResourceManager::_ChangeResourceKey(const tstring& _strOldKey, const tstring& _strNewKey, E_ResourceType _eResourceType)
{
	bool bIsChangedName = false;

	if (_strOldKey == _strNewKey)
		return bIsChangedName;

	SharedPtr<CResource> pResource = nullptr;
	switch (_eResourceType) {
	case E_ResourceType::Prefab: {
		SharedPtr<CPrefab> pPrefab = FindRes<CPrefab>(_strOldKey).Get();
		if (nullptr != pPrefab && pPrefab->GetProtoObj()) {
			// ���� �̸� ����
			tstring strOldFilePath = CPathManager::GetInstance()->GetContentPath() + pPrefab->GetRelativePath();

			tstring strNewPrefabKey = STR_FILE_PATH_Prefab + _strNewKey + _T(".pref");
			tstring strNewFilePath = CPathManager::GetInstance()->GetContentPath() + strNewPrefabKey;
			if (_trename(strOldFilePath.c_str(), strNewFilePath.c_str()) == -1) {
				assert(nullptr && _T("Prefab ���� �̸��� �ٲ� �� �����ϴ�."));
				return bIsChangedName = false;
			}
				

			// ���ҽ��� Key�� �ٲ㼭 �ٽ� ���ҽ��� ����
			pPrefab->SetKey(strNewPrefabKey);
			
			tstring strNewRelativePath = strNewPrefabKey;
			pPrefab->SetRelativePath(strNewRelativePath);
			pPrefab->Save(strNewRelativePath);

			// ���ҽ����� ����, ����
			unordered_map<tstring, CResource*>::iterator it = m_umapResource[(UINT)_eResourceType].find(_strOldKey);
			if (it != m_umapResource[(UINT)_eResourceType].end()) {
				m_umapResource[(UINT)_eResourceType].erase(it);
				AddRes(strNewPrefabKey, pPrefab.Get());
			}
				
			bIsChangedName = true;
		}
	}
		break;
	case E_ResourceType::Material: {
		SharedPtr<CMaterial> pMtrl = FindRes<CMaterial>(_strOldKey).Get();
		if (nullptr != pMtrl && nullptr != pMtrl.Get()) {
			// ���� �̸� ����
			tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
			strFilePath += pMtrl->GetRelativePath();
			tstring strOldFilePath = CPathManager::GetInstance()->GetContentPath() + pMtrl->GetRelativePath();

			tstring strNewKeyName = STR_FILE_PATH_Material + _strNewKey + _T(".mtrl");
			tstring strNewFilePath = CPathManager::GetInstance()->GetContentPath() + strNewKeyName;
				
			if (_trename(strOldFilePath.c_str(), strNewFilePath.c_str()) == -1) {
				assert(nullptr && _T("Material ���� �̸��� �ٲ� �� �����ϴ�."));
				return bIsChangedName = false;
			}
				

			// �̸� �ٲ㼭 �ٽ� ���ҽ��� ����
			pMtrl->SetKey(strNewKeyName);

			tstring strRelativePath = strNewKeyName;
			pMtrl->SetRelativePath(strRelativePath);
			pMtrl->Save(strRelativePath);

			// ���ҽ����� ����, ����
			unordered_map<tstring, CResource*>::iterator it = m_umapResource[(UINT)_eResourceType].find(_strOldKey);
			if (it != m_umapResource[(UINT)_eResourceType].end()) {
				if (it->second)
				m_umapResource[(UINT)_eResourceType].erase(it);
				AddRes(strNewKeyName, pMtrl.Get());
			}
			bIsChangedName = true;
		}
	}
		break;
	case E_ResourceType::GraphicsShader:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CGraphicsShader>(_strOldKey).Get();
		break;
	case E_ResourceType::ComputeShader:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CComputeShader>(_strOldKey).Get();
		break;
	case E_ResourceType::Mesh:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CMesh>(_strOldKey).Get();
		break;
	case E_ResourceType::Texture:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CTexture>(_strOldKey).Get();
		break;
	case E_ResourceType::Sound:
		//pResource = FindRes<CSound>(_strKey).Get();
			// TOOD : �ؾ� ��.
		assert(nullptr && _T("�̿ϼ�"));
		return bIsChangedName;
		break;
	default:
		assert(nullptr && _T("ResourceType : �˼����� enum ��"));
		return bIsChangedName;
	}
	m_bFixed = true;
	return m_bFixed;
}

bool CResourceManager::_DeleteCustomResource(const tstring& _strKey, E_ResourceType _eResourceType)
{
	bool bIsDeleted = false; 
	SharedPtr<CResource> pResource = nullptr;
	switch (_eResourceType) {
	case E_ResourceType::Prefab:
		pResource = FindRes<CPrefab>(_strKey).Get();
		break;
	case E_ResourceType::Material:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CMaterial>(_strKey).Get();
		break;
	case E_ResourceType::GraphicsShader:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CGraphicsShader>(_strKey).Get();
		break;
	case E_ResourceType::ComputeShader:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CComputeShader>(_strKey).Get();
		break;
	case E_ResourceType::Mesh:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CMesh>(_strKey).Get();
		break;
	case E_ResourceType::Texture:
		assert(nullptr && _T("���� ���⿡�� �����ϴ�"));
		pResource = FindRes<CTexture>(_strKey).Get();
		break;
	case E_ResourceType::Sound:
		//pResource = FindRes<CSound>(_strKey).Get();
			// TOOD : �ؾ� ��.
		assert(nullptr && _T("�̿ϼ�"));
		return bIsDeleted;
		break;
	default:
		assert(nullptr && _T("ResourceType : �˼����� enum ��"));
		return bIsDeleted;
	}
		
	if (nullptr == pResource.Get())
		return bIsDeleted;

	// �����Ұ�� �����Ѵ�.

	E_ResourceType eResourceType = pResource->GetResourceType();
	auto iter = m_umapResource[(UINT)eResourceType].begin();
	for (; iter != m_umapResource[(UINT)eResourceType].end(); ++iter) {
		if ((*iter).first == _strKey) {
			m_umapResource[(UINT)eResourceType].erase(iter);
			bIsDeleted = true;
			break;
		}
	}
	m_bFixed = true;

	return bIsDeleted;
}

void CResourceManager::GetResourceKeys(E_ResourceType _eType, vector<tstring>& _vecOut)
{
	_vecOut.empty();
	for (const auto& pair : m_umapResource[(UINT)_eType])
		_vecOut.push_back(pair.first);
}
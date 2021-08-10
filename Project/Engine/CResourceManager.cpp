#include "pch.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

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
}

void CResourceManager::CreateDefaultMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	////////////////
	// RectMesh 생성

	// 버퍼 만들기
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
	|＼     |
	|  ＼   |
	|    ＼ |
	3-------2
	*/

	CMesh* pMesh = new CMesh();
	pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_RectMesh, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);

	///////////////////
	// RectLineMesh 생성
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
	// Point Mesh 생성
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
}

void CResourceManager::CreateDefaultCircle2DMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	// Circle Mesh, Circle Line Mesh 만들기

	// 원점 설정
	VTX vtx = {};

	vtx.vPos = Vector3(0.f, 0.f, 0.f);
	vtx.vUV = Vector2(0.5f, 0.5f);
	Vector4 vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vtx.vColor = vColor;
	vecVtx.push_back(vtx);
	
	float fRadius = 0.5f;
	int iSidesCnt = 40; // 변의 개수
	float fRadian = XM_2PI / (float)iSidesCnt;

	// 테두리의 정점 설정
	for (int i = 0; i < iSidesCnt + 1; ++i) {
		vtx.vPos.x = cosf(fRadian * (float)i) * fRadius;
		vtx.vPos.y = sinf(fRadian * (float)i) * fRadius;
		vtx.vColor = vColor;
		vtx.vUV;
		
		vecVtx.push_back(vtx);
	}

	// 인덱스 설정

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
	// TODO (Jang): 20210606 UV 좌표 값 넣기
	// 
	// 버퍼 만들기
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
		  ＼
	0-------1  |
	|＼     |  |
	|  ＼   |  |
	|   7＼ |--6
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
	// 기본 쉐이더 생성 (AlphaBlend Coveratge)
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
	// 기본 쉐이더 생성 (AlphaBlend)
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
	// Light2D 쉐이더 생성
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
	// Collider2D 쉐이더 생성
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

	AddRes(STR_KEY_Collider2DShader, pShader);

	//-----------------------
	// 타일맵 쉐이더 생성
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
	// 파티클 렌더 쉐이더 생성
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
	// Distortion(왜곡) 쉐이더 생성
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
}

void CResourceManager::CreateDefaultMaterial()
{
	// 기본 재질 생성 (AlphaBlend Coverage)
	CMaterial* pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderAlphaBlendCV = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlend_CoverageShader);
	pMtrl->SetShader(pShaderAlphaBlendCV);
	AddRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl, pMtrl);

	// 기본 재질 생성 (AlphaBlend)
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderAlphaBlend = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlendShader);
	pMtrl->SetShader(pShaderAlphaBlend);
	AddRes<CMaterial>(STR_KEY_StdAlphaBlendMtrl, pMtrl);

	// Light2D 재질 설정
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderLight2D = LoadRes<CGraphicsShader>(STR_KEY_StdLight2DShader);
	pMtrl->SetShader(pShaderLight2D);
	AddRes<CMaterial>(STR_KEY_StdLight2DMtrl, pMtrl);

	// Collider2D 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderCollider2D = LoadRes<CGraphicsShader>(STR_KEY_Collider2DShader);
	pMtrl->SetShader(pShaderCollider2D);
	AddRes(STR_KEY_Collider2DMtrl, pMtrl);

	// 타일맵 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderTileMap = LoadRes<CGraphicsShader>(STR_KEY_TileMapShader);
	pMtrl->SetShader(pShaderTileMap);
	AddRes(STR_KEY_TileMapMtrl, pMtrl);

	// 파티클 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderParticle = LoadRes<CGraphicsShader>(STR_KEY_ParticleShader);
	pMtrl->SetShader(pShaderParticle);
	AddRes(STR_KEY_ParticleMtrl, pMtrl);

	// 왜곡(Distortion) 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderDistortion = LoadRes<CGraphicsShader>(STR_KEY_DistortionShader);
	pMtrl->SetShader(pShaderDistortion);

	SharedPtr<CTexture> pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	AddRes(STR_KEY_DistortionMtrl, pMtrl);

	// FishEye 재질 생성
	pMtrl = new CMaterial(true);
	SharedPtr<CGraphicsShader> pShaderFishEye = LoadRes<CGraphicsShader>(STR_KEY_FishEyeShader);
	pMtrl->SetShader(pShaderFishEye);

	pPostEffectTargetTex = LoadRes<CTexture>(STR_ResourceKey_PostEffectTargetTexture);
	pMtrl->SetData(E_ShaderParam::Texture_0, pPostEffectTargetTex.Get());
	AddRes(STR_KEY_FishEyeMtrl, pMtrl);
}


#include "CTestShader.h"
#include "CParticleUpdateShader.h"
void CResourceManager::CreateComputeShader()
{
	// TODO (Jang) : Test용 컴퓨트 쉐이더를 생성하고있음. 나중에 고치기
	CComputeShader* pShader = new CTestShader;
	pShader->CreateComputeShader(STR_FILE_PATH_TestComputeShader, "CS_Test");
	AddRes(STR_KEY_TestComputeShader, pShader);

	/////////////////////////
	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(STR_FILE_PATH_ParticleUpdateShader, STR_FUNC_NAME_ParticleUpdate);
	AddRes(STR_KEY_ParticleUpdateShader, pShader);
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

void CResourceManager::GetResourceNames(E_ResourceType _eType, vector<tstring>& _vecOut)
{
	_vecOut.empty();
	for (const auto& pair : m_umapResource[(UINT)_eType])
		_vecOut.push_back(pair.first);
}
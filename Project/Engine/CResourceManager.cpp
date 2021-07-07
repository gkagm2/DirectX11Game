#include "pch.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

CResourceManager::CResourceManager() {

}
CResourceManager::~CResourceManager() {
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
	CGraphicsShader* pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	AddRes(STR_KEY_StdAlphaBlend_CoverageShader, pShader);

	// -----------------------
	// �⺻ ���̴� ���� (AlphaBlend)
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend);

	AddRes(STR_KEY_StdAlphaBlendShader, pShader);

	//----------------------
	// Light2D ���̴� ����
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderLight2D);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderLight2D);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	// OM
	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	
	AddRes(STR_KEY_StdLight2DShader, pShader);

	//----------------------
	// Collider2D ���̴� ����
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderCollider2D);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderCollider2D);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	// Topology
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// OM (Output Merge)
	pShader->SetDepthStencilState(E_DepthStencilState::No_Test_No_Write);

	AddRes(STR_KEY_Collider2DShader, pShader);

	//-----------------------
	// Ÿ�ϸ� ���̴� ����
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_VTXShaderTileMap);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNC_NAME_PIXShaderTileMap);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	
	AddRes(STR_KEY_TileMapShader, pShader);

	//-------------------------
	// ��ƼŬ ���� ���̴� ����
	pShader = new CGraphicsShader;
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

	AddRes(STR_KEY_ParticleShader, pShader);
}

void CResourceManager::CreateDefaultMaterial()
{
	// �⺻ ���� ���� (AlphaBlend Coverage)
	CMaterial* pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderAlphaBlendCV = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlend_CoverageShader);
	pMtrl->SetShader(pShaderAlphaBlendCV);
	AddRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl, pMtrl);

	// �⺻ ���� ���� (AlphaBlend)
	pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderAlphaBlend = LoadRes<CGraphicsShader>(STR_KEY_StdAlphaBlendShader);
	pMtrl->SetShader(pShaderAlphaBlend);
	AddRes<CMaterial>(STR_KEY_StdAlphaBlendMtrl, pMtrl);

	// Light2D ���� ����
	pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderLight2D = LoadRes<CGraphicsShader>(STR_KEY_StdLight2DShader);
	pMtrl->SetShader(pShaderLight2D);
	AddRes<CMaterial>(STR_KEY_StdLight2DMtrl, pMtrl);

	// Collider2D ���� ����
	pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderCollider2D = LoadRes<CGraphicsShader>(STR_KEY_Collider2DShader);
	pMtrl->SetShader(pShaderCollider2D);
	AddRes(STR_KEY_Collider2DMtrl, pMtrl);

	// Ÿ�ϸ� ���� ����
	pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderTileMap = LoadRes<CGraphicsShader>(STR_KEY_TileMapShader);
	pMtrl->SetShader(pShaderTileMap);
	AddRes(STR_KEY_TileMapMtrl, pMtrl);

	// ��ƼŬ ���� ����
	pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderParticle = LoadRes<CGraphicsShader>(STR_KEY_ParticleShader);
	pMtrl->SetShader(pShaderParticle);
	AddRes(STR_KEY_ParticleMtrl, pMtrl);

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

void CResourceManager::CreateDefaultTexture()
{
	SharedPtr<CTexture> pTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_FILE_PATH_NoiseTexture1);
	CResourceManager::GetInstance()->LoadRes<CTexture>(STR_FILE_PATH_NoiseTexture2);
	g_globalConst.vNoiseResolution = Vector2((float)pTexture->GetDimension().x, (float)pTexture->GetDimension().y);
}

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

	return pTexture;
}

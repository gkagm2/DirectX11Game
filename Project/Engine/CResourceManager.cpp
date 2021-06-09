#include "pch.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

CResourceManager::CResourceManager() {

}
CResourceManager::~CResourceManager() {
	for (UINT i = 0; i < (UINT)E_ResourceType::End; ++i)
		Safe_Delete_UnorderedMap(m_umapResource[i]);
}

void CResourceManager::Init()
{
	CreateDefaultMesh();
	//CreateDefaultCubeMesh3D();
	CreateDefaultShader();
	CreateDefaultMaterial();
}

void CResourceManager::CreateDefaultMesh()
{
	// ���� �����
	VTX vertices[] = {
		{	Vector3(-1.f, 1.f, 0.f), 
			Vector4(0.0f, 0.0f, 1.0f, 1.0f), 
			Vector2(0.f,0.f) },
		{	Vector3(1.f, 1.f, 0.f), 
			Vector4(0.0f, 1.0f, 0.0f, 1.0f), 
			Vector2(1.f,0.f) },
		{	Vector3(1.f, -1.f, 0.f), 
			Vector4(0.0f, 1.0f, 1.0f, 1.0f), 
			Vector2(1.f,1.f) },
		{	Vector3(-1.f, -1.f, 0.f), 
			Vector4(1.0f, 0.0f, 0.0f, 1.0f), 
			Vector2(0.f,1.f) }
	};


	// create index buffer
	UINT arrIdx[6] = { 
		0,1,2,
		0,2,3 };
	/*
	0-------1
	|��     |
	|  ��   |
	|    �� |
	3-------2
	*/

	CMesh* pMesh = new CMesh();
	pMesh->Create(vertices, sizeof(VTX) * 4, arrIdx, sizeof(UINT) * 6, D3D11_USAGE_DEFAULT);

	AddRes(STR_KEY_RectMash, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);
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

	AddRes(STR_KEY_CubeMash, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);
}

void CResourceManager::CreateDefaultShader()
{
	// �⺻ ���̴� ���� (AlphaBlend Coveratge)
	CGraphicsShader* pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNCTION_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNCTION_NAME_PIXShader);

	// Rasterizer
	pShader->SetRasterizerState(E_RasterizerState::CullNone);

	pShader->SetBlendState(E_BlendState::AlphaBlend_Coverage);
	AddRes(STR_KEY_StandardShaderAlphaBlend_Coverage, pShader);

	///////////////////////////////////////////////

	// �⺻ ���̴� ���� (AlphaBlend)
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNCTION_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNCTION_NAME_PIXShader);

	pShader->SetRasterizerState(E_RasterizerState::CullNone);
	pShader->SetBlendState(E_BlendState::AlphaBlend);

	AddRes(STR_KEY_StandardShaderAlphaBlend, pShader);
}

void CResourceManager::CreateDefaultMaterial()
{
	// �⺻ ���� ���� (AlphaBlend Coverage)
	CMaterial* pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderAlphaBlendCV = FindRes<CGraphicsShader>(STR_KEY_StandardShaderAlphaBlend_Coverage);
	pMtrl->SetShader(pShaderAlphaBlendCV);
	AddRes<CMaterial>(STR_KEY_StandardMaterialAlphaBlend_Coverage, pMtrl);

	// �⺻ ���� ���� (AlphaBlend)
	pMtrl = new CMaterial;
	SharedPtr<CGraphicsShader> pShaderAlphaBlend = FindRes<CGraphicsShader>(STR_KEY_StandardShaderAlphaBlend);
	pMtrl->SetShader(pShaderAlphaBlend);
	AddRes<CMaterial>(STR_KEY_StandardMaterialAlphaBlend, pMtrl);
}
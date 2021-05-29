#include "pch.h"
#include "CResourceManager.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

CResourceManager::CResourceManager() {

}
CResourceManager::~CResourceManager() {
	for (UINT i = 0; i < (UINT)E_ResourceType::END; ++i)
		Safe_Delete_UnorderedMap(m_umapResource[i]);
}

void CResourceManager::Init()
{
	CreateDefaultMesh();
	CreateDefaultShader();
}

void CResourceManager::CreateDefaultMesh()
{
	// 버퍼 만들기

	
	VTX vertices[4] = {};
	float fScale = 0.8f;
	vertices[0].vPos = Vector3(-1.f * fScale, -1.f * fScale, 0.f);
	vertices[0].vColor = Vector4{ 1.f,0.f,0.f,1.f };

	vertices[1].vPos = Vector3(-1.f * fScale, 1.f * fScale, 0.f);
	vertices[1].vColor = Vector4{ 0.f,1.f,0.f,1.f };

	vertices[2].vPos = Vector3(1.f * fScale, 1.f * fScale, 0.f);
	vertices[2].vColor = Vector4{ 0.f,0.f,1.f,1.f };

	vertices[3].vPos = Vector3(1.f * fScale, -1.f * fScale, 0.f);
	vertices[3].vColor = Vector4{ 0.f,0.f,1.f,1.f };

	// create index buffer
	UINT arrIdx[6] = { 0 ,1, 3, 3, 1, 2 };
	/*
	1-------2
	|＼     |
	|  ＼   |
	|    ＼ |
	0-------3
	*/

	CMesh* pMesh = new CMesh();
	pMesh->Create(vertices, sizeof(VTX) * 4, arrIdx, sizeof(UINT) * 6, D3D11_USAGE_DEFAULT);

	AddResource(STR_KEY_RectMash, pMesh); // AddResource<CMesh>(STR_KEY_RectMash, pMesh);
}

void CResourceManager::CreateDefaultShader()
{
	// 기본 쉐이더 생성
	CGraphicsShader* pShader = new CGraphicsShader();
	pShader->CreateVertexShader(STR_FILE_PATH_Shader, STR_FUNCTION_NAME_VTXShader);
	pShader->CreatePixelShader(STR_FILE_PATH_Shader, STR_FUNCTION_NAME_PIXShader);

	AddResource(STR_KEY_StandardShader, pShader);
}
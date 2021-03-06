#include "pch.h"
#include "CTileMapGrid.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CTileMap.h"

CTileMapGrid::CTileMapGrid(CTileMap* _pTargetTileMap) :
	m_pTargetTileMap(_pTargetTileMap),
	m_vGridColor{ 1.f,0.f,1.f,1.f }
{
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_LineListMtrl);
}

CTileMapGrid::CTileMapGrid(const CTileMapGrid& _origin) :
	m_pMtrl(_origin.m_pMtrl),
	m_pTargetTileMap(nullptr),
	m_vGridColor{_origin.m_vGridColor}
{
	Init();
}

CTileMapGrid::~CTileMapGrid()
{
	Safe_Delete_Vector(m_vecMesh);
}

void CTileMapGrid::Init()
{
	if (!m_pTargetTileMap) {
		return;
	}
	if (!m_pTargetTileMap->GetGameObject()) {
		return;
	}
	if (!m_pTargetTileMap->Transform()) {
		return;
	}
	m_bDidInit = true;
	Clear();
	// gridsize를 받아와야 함.
	// width, height을 받아와야 함.
	int iCol, iRow;
	Vector3 vTileMapScale = m_pTargetTileMap->Transform()->GetScale(); // width height
	Vector3 vObjPos = m_pTargetTileMap->Transform()->GetPosition();
	iRow = m_pTargetTileMap->GetRow();
	iCol = m_pTargetTileMap->GetCol();

	Vector2 vGridSize{ 1.f,1.f };
	Vector3 vScale = vTileMapScale;
	Vector3 vOriginLBPos = /*vObjPos*/ - vTileMapScale * 0.5f; // 좌하단 좌표
	Vector3 vOriginRTPos = /*vObjPos*/ + vTileMapScale * 0.5f; // 우상단 좌표

	vGridSize = Vector2(vScale.x / iCol, vScale.y / iRow);

	for (int y = 0; y < iRow + 1; ++y) {
		Vector3 vStartPos = Vector3(vOriginLBPos.x, vOriginLBPos.y + y * vGridSize.y, vObjPos.z);
		Vector3 vEndPos = Vector3(vOriginRTPos.x, vOriginLBPos.y + y * vGridSize.y, vObjPos.z);

		
		AddBuffer(vStartPos / vScale, vEndPos / vScale); // 1 미만 사이즈로 만들어줌.
	}
	for (int x = 0; x < iCol + 1; ++x) {
		Vector3 vStartPos = Vector3(vOriginLBPos.x + x * vGridSize.x, vOriginLBPos.y, vObjPos.z);
		Vector3 vEndPos = Vector3(vOriginLBPos.x + x * vGridSize.x, vOriginRTPos.y, vObjPos.z);
		AddBuffer(vStartPos / vScale, vEndPos / vScale); // 1 미만 사이즈로 만들어줌.
	}
}

void CTileMapGrid::AddBuffer(const Vector3& _vPos1, const Vector3& _vPos2)
{
	VTX vertex = {};
	vertex.vPos = _vPos1;
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	m_vecVtx.push_back(vertex);
	m_vecIdx.push_back(0);

	vertex.vPos = _vPos2;
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	m_vecVtx.push_back(vertex);
	m_vecIdx.push_back(1);

	CMesh* pMesh = new CMesh;
	pMesh->Create(m_vecVtx.data(), sizeof(VTX) * (UINT)m_vecVtx.size(), m_vecIdx.data(), sizeof(UINT) * (UINT)m_vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
	m_vecMesh.push_back(pMesh);

	m_vecVtx.clear();
	m_vecIdx.clear();
}

void CTileMapGrid::Clear()
{
	Safe_Delete_Vector(m_vecMesh);
}

void CTileMapGrid::UpdateData()
{
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, &m_vGridColor);
	m_pMtrl->UpdateData();
	for (size_t i = 0; i < m_vecMesh.size(); ++i)
		m_vecMesh[i]->Render(0); // TODO (Jang) : Instancing Rendering을 바꾸기
}
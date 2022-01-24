#include "pch.h"
#include "CLine.h"
#include "CResourceManager.h"

CLine::CLine() :
	m_pMtrl{ nullptr },
	m_vLineColor{ 0.2f, 0.2f, 0.8f, 1.f }
{
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_LineListMtrl);
}

CLine::~CLine()
{
	Safe_Delete_Vector(m_vecMesh);
}

void CLine::Init()
{
}

void CLine::AddLine(const Vector3& _vPos1, const Vector3& _vPos2)
{


	AddBuffer(_vPos1, _vPos2);
}

void CLine::AddBuffer(const Vector3& _vPos1, const Vector3& _vPos2)
{
	VTX vertex = {};
	vertex.vPos = _vPos1;
	vertex.vColor = Vector4(1.f,1.f,1.f,1.f);
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

void CLine::Clear()
{
	Safe_Delete_Vector(m_vecMesh);
}

void CLine::UpdateData()
{
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, &m_vLineColor);
	m_pMtrl->UpdateData();
	for (size_t i = 0; i < m_vecMesh.size(); ++i)
		m_vecMesh[i]->Render(); // TODO (Jang) : Instancing Rendering으로 바꾸기
}
#include "pch.h"
#include "CGrid.h"
#include "CMesh.h"
#include "CResourceManager.h"

CGrid::CGrid(CGameObject* _pTargetObj) :
	pTargetObj(_pTargetObj),
	m_iHeight{30},
	m_iWidth{30}
{
	InitBuffer();
	Init(m_iHeight, m_iWidth);
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_LineListMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_LineListMtrl);
}

CGrid::~CGrid()
{
}

void CGrid::Init(int _iWidth, int _iHeight)
{
	VTX vertex = {};
	m_vecVtx.clear();
	m_vecIdx.clear();

	vertex.vPos = Vector3(0.f, 0.f, 0.f);
	vertex.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	vertex.vUV = Vector2(0.5, 0.5f);

	int i= 0;
	for (int h = 0; h < _iHeight; ++h) {
		for (int w = 0; w < _iWidth; ++w) {
			vertex.vPos.x = ((float)w / _iWidth) * 30.f;
			vertex.vPos.y = ((float)h / _iHeight) * 30.f;
			m_vecVtx.push_back(vertex);
			m_vecIdx.push_back(i);

			
			if (i == 2) {
				m_pMesh = new CMesh;
				m_pMesh->Create(m_vecVtx.data(), sizeof(VTX) * (UINT)m_vecVtx.size(), m_vecIdx.data(), sizeof(UINT) * (UINT)m_vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
				m_vecMesh.push_back(m_pMesh);

				m_vecVtx.clear();
				m_vecIdx.clear();
				i = 0;
			}
			++i;
		}
	}
}

void CGrid::InitBuffer()
{
}

void CGrid::UpdateData()
{
	m_pMtrl->UpdateData();
	for (int i = 0; i < m_vecMesh.size(); ++i)
		m_vecMesh[i]->Render();
} 
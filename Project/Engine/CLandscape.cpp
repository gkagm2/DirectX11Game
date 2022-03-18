#include "pch.h"
#include "CLandscape.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CResourceManager.h"

CLandscape::CLandscape() :
	CRenderer(E_ComponentType::Landscape),
	m_iQuadX(7),
	m_iQuadZ(7),
	m_iComponentX(1),
	m_iComponentZ(1)
{
}

CLandscape::~CLandscape()
{
}

void CLandscape::FinalUpdate()
{
}

void CLandscape::Render()
{
	Transform()->UpdateData();
	m_pMtrl->UpdateData();
	m_pMesh->Render();
}

void CLandscape::Create()
{
	tstring strKey = {};// FindResName<CMesh>(_T("LandscapeMesh"));

	// 메쉬 만들기
	
	// 기존에 참조하던 메쉬는 삭제
	if (nullptr != m_pMesh) {
		CResourceManager::GetInstance()->ForceDeleteRes<CMesh>(m_pMesh->GetKey());
		m_pMesh = nullptr;
	}
	
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	VTX v;
	UINT iFaceX = m_iComponentX * m_iQuadX;
	UINT iFaceZ = m_iComponentZ * m_iQuadZ;

	UINT iVtxX = iFaceX + 1;
	UINT iVtxZ = iFaceZ + 1;

	// vertex setting
	for (UINT iZ = 0; iZ < iVtxZ; ++iZ) {
		for (UINT iX = 0; iX < iVtxX; ++iX) {
			v.vPos = Vector3(iX, 0, iZ);
			v.vUV = Vector2(iX, iFaceZ - iZ);

			v.vNormal = Vector3::Up;
			v.vTangent = Vector3::Right;
			v.vBinormal = Vector3::Back;

			v.vColor = Vector4(1.f, 0.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	// index setting
	for (UINT iZ = 0; iZ < iFaceZ; ++iZ) {
		for (UINT iX = 0; iX < iFaceX; ++iX) {
			// 0
			// | \
			// 2- 1
			vecIdx.push_back(iZ * iVtxX + iX + iVtxX);
			vecIdx.push_back(iZ * iVtxX + iX + 1);
			vecIdx.push_back(iZ * iVtxX + iX);

			// 1- 2
			//  \ |
			//    0
			vecIdx.push_back(iZ * iVtxX + iX + 1);
			vecIdx.push_back(iZ * iVtxX + iX + iVtxX);
			vecIdx.push_back(iZ * iVtxX + iX + iVtxX + 1);
		}
	}

	m_pMesh = new CMesh;
	m_pMesh->Create(vecVtx.data(), sizeof(VTX) * (UINT)vecVtx.size(), vecIdx.data(), sizeof(UINT) * (UINT)vecIdx.size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);

	// TODO (Jang) : Key 값 자동 생성하기
	tstring strNewKey = _T("LandscapeMesh"); // auto generate key
	CResourceManager::GetInstance()->AddRes<CMesh>(strNewKey, m_pMesh.Get());

	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_LandscapeMtrl);
	assert(m_pMtrl.Get());
}

bool CLandscape::SaveToScene(FILE* _pFile)
{
	CRenderer::SaveToScene(_pFile);

	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	SaveResourceToFile(m_pTex, _pFile);
	SaveResourceToFile(m_pWeightMapTex, _pFile);

	FWrite(m_iQuadX, _pFile);
	FWrite(m_iQuadZ, _pFile);
	FWrite(m_iComponentX, _pFile);
	FWrite(m_iComponentZ, _pFile);

	return true;
}

bool CLandscape::LoadFromScene(FILE* _pFile)
{
	CRenderer::LoadFromScene(_pFile);

	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	LoadResourceFromFile(m_pTex, _pFile);
	LoadResourceFromFile(m_pWeightMapTex, _pFile);

	FRead(m_iQuadX, _pFile);
	FRead(m_iQuadZ, _pFile);
	FRead(m_iComponentX, _pFile);
	FRead(m_iComponentZ, _pFile);
	
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	LoadResourceFromFile(m_pTex, _pFile);
	LoadResourceFromFile(m_pWeightMapTex, _pFile);

	FRead(m_iQuadX, _pFile);
	FRead(m_iQuadZ, _pFile);
	FRead(m_iComponentX, _pFile);
	FRead(m_iComponentZ, _pFile);

	return true;
}
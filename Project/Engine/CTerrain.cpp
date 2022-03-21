#include "pch.h"
#include "CTerrain.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CRenderManager.h"
#include "CStructuredBuffer.h"
#include "CResourceManager.h"
#include "CFontManager.h"

CTerrain::CTerrain() :
	CRenderer(E_ComponentType::Terrain),
	m_iQuadX(7),
	m_iQuadZ(7),
	m_iFaceX(7),
	m_iFaceZ(7),
	m_iComponentX(1),
	m_iComponentZ(1)
{
	m_pCrossBuffer = make_unique<CStructuredBuffer>();
	m_pCrossBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TRaycastOut), 1, true, nullptr);
}

CTerrain::CTerrain(const CTerrain& _origin) :
	CRenderer(E_ComponentType::Terrain),
	m_iQuadX(_origin.m_iQuadX),
	m_iQuadZ(_origin.m_iQuadZ),
	m_iFaceX(_origin.m_iFaceX),
	m_iFaceZ(_origin.m_iFaceZ),
	m_iComponentX(_origin.m_iComponentX),
	m_iComponentZ(_origin.m_iComponentZ)
{
	m_pCrossBuffer = make_unique<CStructuredBuffer>();
	m_pCrossBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TRaycastOut), 1, true, nullptr);
}

CTerrain::~CTerrain()
{
	SAFE_DELETE_PTR(m_pMtrl);
}


void CTerrain::FinalUpdate()
{
	// RayCasting
	CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
	if (nullptr == pMainCam)
		return;

	Vector3 vMainCamPos = pMainCam->Transform()->GetPosition();
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, &vMainCamPos);

	const Matrix& matWorldInv = Transform()->GetWorldInverseMatrix();
	const TRay& ray = pMainCam->GetRay();

	TRay CamRay = {};
	CamRay.vStartPos= XMVector3TransformCoord(ray.vStartPos, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	TRaycastOut out = { Vector2(0.f, 0.f), 0x7fffffff, 0 }; // 최상위 비트만 0
	m_pCrossBuffer->SetData(&out, 1);

	m_pCSRaycast->SetHeightMap(m_pHeightMapTex);
	m_pCSRaycast->SetFaceCount(m_iFaceX, m_iFaceZ);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer.get());

	m_pCSRaycast->Excute();

	TRaycastOut output = {};
	m_pCrossBuffer->GetData(&output, sizeof(TRaycastOut));

	wchar_t szBuffer[255] = {};
	swprintf_s(szBuffer, L"Picking UV (%f, %f)", output.vUV.x, output.vUV.y);
	CFontManager::GetInstance()->DrawUIFont(szBuffer, 10, 60, 12, FONT_RGBA(255, 20, 20, 127), FW1_TEXT_FLAG::FW1_LEFT);
}

void CTerrain::Render()
{
	if (nullptr == m_pMtrl || nullptr == m_pMesh)
		return;
	Transform()->UpdateData();
	m_pMtrl->UpdateData();
	m_pMesh->Render();
}

void CTerrain::Create()
{
	tstring strKey = {};// FindResName<CMesh>(_T("TerrainMesh"));

	// 메쉬 만들기
	
	// 기존에 참조하던 메쉬는 삭제
	if (nullptr != m_pMesh) {
		CResourceManager::GetInstance()->ForceDeleteRes<CMesh>(m_pMesh->GetKey());
		m_pMesh = nullptr;
	}
	
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	VTX v;
	m_iFaceX = m_iComponentX * m_iQuadX;
	m_iFaceZ = m_iComponentZ * m_iQuadZ;

	UINT iVtxX = m_iFaceX + 1;
	UINT iVtxZ = m_iFaceZ + 1;

	// vertex setting
	for (UINT iZ = 0; iZ < iVtxZ; ++iZ) {
		for (UINT iX = 0; iX < iVtxX; ++iX) {
			v.vPos = Vector3(iX, 0, iZ);
			v.vUV = Vector2(iX, m_iFaceZ - iZ);

			v.vNormal = Vector3::Up;
			v.vTangent = Vector3::Right;
			v.vBinormal = Vector3::Back;

			v.vColor = Vector4(1.f, 0.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	// index setting
	for (UINT iZ = 0; iZ < m_iFaceZ; ++iZ) {
		for (UINT iX = 0; iX < m_iFaceX; ++iX) {
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
	tstring strNewKey = _T("TerrainMesh"); // auto generate key
	CResourceManager::GetInstance()->AddRes<CMesh>(strNewKey, m_pMesh.Get());

	// TODO (Jang) : Save 되는 문제 발생.
	if (nullptr == m_pMtrl) {
		SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_TerrainMtrl);
		assert(pMtrl.Get());
		m_pMtrl = pMtrl->Clone_NoAddInResMgr();
	}

	m_pMtrl->SetData(E_ShaderParam::Int_0, &m_iFaceX);
	m_pMtrl->SetData(E_ShaderParam::Int_1, &m_iFaceZ);

	SetHeightMapTex(m_pHeightMapTex);

	// 지형 피킹 컴퓨트 쉐이더
	m_pCSRaycast = (CRaycastShader*)CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_RaycastShader).Get();
}

void CTerrain::SetHeightMapTex(SharedPtr<CTexture> m_pTex)
{
	m_pHeightMapTex = m_pTex;
	if (nullptr != m_pHeightMapTex) {
		Vector2 vHeightMapResolution = m_pHeightMapTex->GetResolution();
		m_pMtrl->SetData(E_ShaderParam::Vector2_0, &vHeightMapResolution);
		m_pMtrl->SetData(E_ShaderParam::Texture_0, m_pHeightMapTex.Get());
	}
}

void CTerrain::SetWeightMapTex(SharedPtr<CTexture> m_pTex)
{
	m_pWeightMapTex = m_pTex;
	if (nullptr != m_pWeightMapTex)
		m_pMtrl->SetData(E_ShaderParam::Texture_1, m_pWeightMapTex.Get());
}

bool CTerrain::SaveToScene(FILE* _pFile)
{
	CRenderer::SaveToScene(_pFile);

	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	SaveResourceToFile(m_pHeightMapTex, _pFile);
	SaveResourceToFile(m_pWeightMapTex, _pFile);

	FWrite(m_iQuadX, _pFile);
	FWrite(m_iQuadZ, _pFile);
	FWrite(m_iComponentX, _pFile);
	FWrite(m_iComponentZ, _pFile);
	FWrite(m_iFaceX, _pFile);
	FWrite(m_iFaceZ, _pFile);

	return true;
}

bool CTerrain::LoadFromScene(FILE* _pFile)
{
	CRenderer::LoadFromScene(_pFile);

	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	LoadResourceFromFile(m_pHeightMapTex, _pFile);
	LoadResourceFromFile(m_pWeightMapTex, _pFile);

	FRead(m_iQuadX, _pFile);
	FRead(m_iQuadZ, _pFile);
	FRead(m_iComponentX, _pFile);
	FRead(m_iComponentZ, _pFile);
	FRead(m_iFaceX, _pFile);
	FRead(m_iFaceZ, _pFile);

	return true;
}
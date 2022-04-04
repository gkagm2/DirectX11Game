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
	m_vBrushScale(Vector2(0.1f,0.1f)),
	m_iBrushIdx(0),
	m_iQuadX(7),
	m_iQuadZ(7),
	m_iFaceX(7),
	m_iFaceZ(7),
	m_iComponentX(1),
	m_iComponentZ(1),
	m_eTerrainMode(E_TerrainMode::NONE),
	m_iWeightWidth{ 0 },
	m_iWeightHeight{ 0 },
	m_iWeightIdx{ 0 },
	m_pWeightMapBuffer{ nullptr },
	m_fTileCnt{ 3.f }
{
	m_pCrossBuffer = new  CStructuredBuffer;
	m_pCrossBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TRaycastOut), 1, true, nullptr);
}

CTerrain::CTerrain(const CTerrain& _origin) :
	CRenderer(E_ComponentType::Terrain),
	m_vBrushScale(_origin.m_vBrushScale),
	m_iBrushIdx(_origin.m_iBrushIdx),
	m_iQuadX(_origin.m_iQuadX),
	m_iQuadZ(_origin.m_iQuadZ),
	m_iFaceX(_origin.m_iFaceX),
	m_iFaceZ(_origin.m_iFaceZ),
	m_iComponentX(_origin.m_iComponentX),
	m_iComponentZ(_origin.m_iComponentZ),
	m_eTerrainMode(E_TerrainMode::NONE),
	m_iWeightWidth{ _origin.m_iWeightWidth},
	m_iWeightHeight{ _origin.m_iWeightHeight },
	m_iWeightIdx{ _origin.m_iWeightIdx },
	m_pWeightMapBuffer{ nullptr },
	m_fTileCnt{_origin.m_fTileCnt}
{
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TRaycastOut), 1, true, nullptr);
}

CTerrain::~CTerrain()
{
	SAFE_DELETE_PTR(m_pMtrl);
	SAFE_DELETE(m_pCrossBuffer);
	SAFE_DELETE(m_pWeightMapBuffer);
}


void CTerrain::FinalUpdate()
{
	if (m_eTerrainMode == E_TerrainMode::NONE)
		return;

	_Raycasting();

	TRaycastOut output = {};
	m_pCrossBuffer->GetData(&output, sizeof(TRaycastOut));

	wchar_t szBuffer[255] = {};
	swprintf_s(szBuffer, L"Picking UV (%f, %f)", output.vUV.x, output.vUV.y);
	CFontManager::GetInstance()->DrawUIFont(szBuffer, 10, 60, 12, FONT_RGBA(255, 20, 20, 127), FW1_TEXT_FLAG::FW1_LEFT);


	if (E_TerrainMode::HeightMap == m_eTerrainMode) {
		// 교점 위치정보를 토대로 높이를 수정함
		m_pCSHeightMap->SetBrushTexture(m_pBrushArrTex);
		m_pCSHeightMap->SetBrushIdx(m_iBrushIdx);
		m_pCSHeightMap->SetBrushScale(m_vBrushScale); // 브러쉬 크기
		m_pCSHeightMap->SetHeigtMapTexture(m_pHeightMapTex);
		m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 레이 캐스트 위치

		m_pCSHeightMap->Excute();
	}
	else if (E_TerrainMode::Splat == m_eTerrainMode) {
		// 교점 위치정보를 토대로 가중치를 수정함
		m_pCSWeightMap->SetBrushArrTex(m_pBrushArrTex);
		m_pCSWeightMap->SetBrushIdx(m_iBrushIdx);
		m_pCSWeightMap->SetBrushScale(m_vBrushScale);
		m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // 가중치맵, 가로 세로 개수
		m_pCSWeightMap->SetRaycasatInputBuffer(m_pCrossBuffer);
		m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
		m_pCSWeightMap->Excute();
	}
}

void CTerrain::Render()
{
	if (nullptr == m_pMtrl || nullptr == m_pMesh)
		return;
	Transform()->UpdateData();
	
	m_pWeightMapBuffer->UpdateData(17, E_ShaderStage::Pixel);
	// FIXED (Jang) : 테스트 하고 멤버변수로 넣기.

	Vector2 vWeightMapResolution = Vector2((float)m_iWeightWidth, (float)m_iWeightHeight);
	m_pMtrl->SetData(E_ShaderParam::Vector2_1, &vWeightMapResolution);
	m_pMtrl->SetData(E_ShaderParam::TextureArr_0, m_pTileArrTex.Get()); // 타일 배열 텍스쳐
	m_pMtrl->SetData(E_ShaderParam::Float_0, &m_fTileCnt);
	m_pMtrl->UpdateData();

	m_pMesh->Render();
}

void CTerrain::Create()
{
	tstring strKey = FindResNameIfNoExistRetNewName<CMesh>(_T("TerrainMesh")); // auto generate key

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

	CResourceManager::GetInstance()->AddRes<CMesh>(strKey, m_pMesh.Get());

	// 지형 피킹 컴퓨트 쉐이더
	m_pCSRaycast = dynamic_cast<CRaycastShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_RaycastShader).Get());
	assert(m_pCSRaycast.Get());

	// 높이맵 컴퓨트 쉐이더
	m_pCSHeightMap = dynamic_cast<CHeightMapShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_HeightMapShader).Get());
	assert(m_pCSHeightMap.Get());

	CTexture* pHeightMapTex = nullptr;
	// type 1: 높이맵 텍스쳐 세팅 (높이맵 텍스쳐로 세팅)
	//pHeightMapTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("HeightMapTex"), _T("texture\\heightmap.jpg")).Get();
	
	// type 2: 높이맵 텍스쳐 세팅 (자체 세팅)
	if (nullptr == m_pHeightMapTex) {
		tstring strHeightMapTex = FindResNameIfNoExistRetNewName<CTexture>(_T("_HeightMapTex"));
		pHeightMapTex = CResourceManager::GetInstance()->CreateTexture(strHeightMapTex,
			2048, /*width*/
			2048, /*height*/
			DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT,
			D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS).Get();
	}

	// 가중치맵 컴퓨트 쉐이더
	m_pCSWeightMap = dynamic_cast<CWeightMapShader*>(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_WeightMapShader).Get());

	// 가중치 버퍼
	m_iWeightWidth = 1024;
	m_iWeightHeight = 1024;
	
	SAFE_DELETE(m_pWeightMapBuffer);
	m_pWeightMapBuffer = new CStructuredBuffer;
	m_pWeightMapBuffer->Create(E_StructuredBufferType::Read_Write, sizeof(TWeight4), m_iWeightWidth * m_iWeightHeight, false, nullptr);

	// 브러쉬 텍스쳐 세팅
	m_pBrushArrTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("texture\\brush\\Brush_02.png"));

	// 타일 텍스쳐
	//SetTileArrTexture(CResourceManager::GetInstance()->LoadRes<CTexture>(L"TileTex", L"texture\\tile\\TILE_ARR.dds"));
	SetTileArrTexture(CResourceManager::GetInstance()->LoadTexture(_T("TileTex"), _T("texture\\tile\\TILE_ARR.dds"), 8));

	// TODO (Jang) : Save 되는 문제 발생.
	if (nullptr == m_pMtrl) {
		SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_TerrainMtrl);
		assert(pMtrl.Get());
		m_pMtrl = pMtrl->Clone_NoAddInResMgr();
	}

	SetHeightMapTex(pHeightMapTex);
	//SetWeightMapTex()
	
	m_pMtrl->SetData(E_ShaderParam::Int_0, &m_iFaceX);
	m_pMtrl->SetData(E_ShaderParam::Int_1, &m_iFaceZ);
}

void CTerrain::SetHeightMapTex(SharedPtr<CTexture> _pTex)
{
	m_pHeightMapTex = _pTex;
	if (nullptr != m_pHeightMapTex) {
		Vector2 vHeightMapResolution = m_pHeightMapTex->GetResolution();
		m_pMtrl->SetData(E_ShaderParam::Vector2_0, &vHeightMapResolution);
		m_pMtrl->SetData(E_ShaderParam::Texture_0, m_pHeightMapTex.Get());
	}
}

void CTerrain::SetWeightMapTex(SharedPtr<CTexture> _pTex)
{
	m_pWeightMapTex = _pTex;
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
	SaveResourceToFile(m_pBrushArrTex, _pFile);

	FWrite(m_vBrushScale, _pFile);
	FWrite(m_iBrushIdx, _pFile);
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
	LoadResourceFromFile(m_pBrushArrTex, _pFile);

	FRead(m_vBrushScale, _pFile);
	FRead(m_iBrushIdx, _pFile);
	FRead(m_iQuadX, _pFile);
	FRead(m_iQuadZ, _pFile);
	FRead(m_iComponentX, _pFile);
	FRead(m_iComponentZ, _pFile);
	FRead(m_iFaceX, _pFile);
	FRead(m_iFaceZ, _pFile);

	return true;
}

void CTerrain::_Raycasting()
{
	CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
	if (nullptr == pMainCam)
		return;

	Vector3 vMainCamPos = pMainCam->Transform()->GetPosition();
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, &vMainCamPos);

	const Matrix& matWorldInv = Transform()->GetWorldInverseMatrix();
	const TRay& ray = pMainCam->GetRay();

	TRay CamRay = {};
	CamRay.vStartPos = XMVector3TransformCoord(ray.vStartPos, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	TRaycastOut out = { Vector2(0.f, 0.f), 0x7fffffff, 0 };
	m_pCrossBuffer->SetData(&out, sizeof(TRaycastOut));

	m_pCSRaycast->SetHeightMap(m_pHeightMapTex);
	m_pCSRaycast->SetFaceCount(m_iFaceX, m_iFaceZ);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Excute();
}
#include "pch.h"
#include "CRenderManager.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CSceneManager.h"
#include "CStructuredBuffer.h"

CRenderManager::CRenderManager() :
	m_pLight2DBuffer(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	if (m_pLight2DBuffer)
		delete m_pLight2DBuffer;
}
void CRenderManager::Init()
{
	m_pLight2DBuffer = new CStructuredBuffer;
	const UINT iDefaultElementCnt = 5;
	m_pLight2DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iDefaultElementCnt);
}

void CRenderManager::Render()
{
	_RenderInit_Light2D();
	// Render
	// 1. 타겟 클리어
	CDevice::GetInstance()->ClearTarget();

	// TODO : Scene에서도 Render함수를 실행하고 RenderManager에서도 Render를 실행한다. (2번 호출) 이걸 줄여야됨
	// 타일맵 렌더링
	for (UINT i = 0; i < m_vecTileMap.size(); ++i)
		m_vecTileMap[i]->Render();

	// 2. 카메라 기준으로 렌더링
	for (UINT i = 0; i < m_vecCam.size(); ++i)
		m_vecCam[i]->Render();

	// 3. 백버퍼 교체
	CDevice::GetInstance()->Present();

	m_vecCam.clear();
	m_vecLight2D.clear();
	m_vecTileMap.clear();
}

void CRenderManager::_RenderInit_Light2D()
{
	g_globalConst.iLight2DCount = (int)m_vecLight2D.size();

	// 컨스트 버퍼에 올려서 GPU에 넣자.
	static const CConstBuffer* pLight2DBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Global);
	pLight2DBuffer->SetData(&g_globalConst);
	pLight2DBuffer->UpdateData();


	UINT iLightSize = (UINT)m_vecLight2D.size();
	UINT iElementCnt = m_pLight2DBuffer->GetElementCount();

	// 광원 개수가 광원의 정보를 담을 구조화 버퍼 개수보다 크면 사이즈를 늘려줌
	if (m_pLight2DBuffer->GetElementCount() < iLightSize) {
		m_pLight2DBuffer->Release();
		m_pLight2DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iElementCnt);
	}

	vector<TLightInfo> vecLightInfo;
	for (UINT i = 0; i < iLightSize; ++i)
		vecLightInfo.push_back(m_vecLight2D[i]->GetLightInfo());

	m_pLight2DBuffer->SetData(vecLightInfo.data(), (UINT)vecLightInfo.size());
	m_pLight2DBuffer->UpdateData(REGISTER_NUM_Light2DBuffer);
}
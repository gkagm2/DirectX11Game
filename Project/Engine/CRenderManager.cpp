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
}

void CRenderManager::Init()
{
	m_pLight2DBuffer = make_unique<CStructuredBuffer>();
	const UINT iDefaultElementCnt = 5;
	m_pLight2DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iDefaultElementCnt);
}

void CRenderManager::Render()
{
	_RenderInit_Light2D();
	// Render
	// 1. 타겟 클리어
	CDevice::GetInstance()->ClearTarget();

	E_SceneMode eSceneMode = CSceneManager::GetInstance()->GetSceneMode();
	switch (eSceneMode) {
	case E_SceneMode::Play:
		_RenderInGame();
		// TODO (Jang) : InGame Camera, Tool Camera를 나눴을 때 만들기
		//_RenderTool();
	case E_SceneMode::Stop:
	case E_SceneMode::Pause:
		_RenderTool();
		break;
	default:
		assert(nullptr);
		break;
	}

	// 타일맵 렌더링
	for (UINT i = 0; i < m_vecTileMap.size(); ++i)
		m_vecTileMap[i]->Render();

	_RenderClear();
	
}

void CRenderManager::_RenderInGame()
{
	// 2. 카메라 기준으로 렌더링
	for (UINT i = 0; i < m_vecCam.size(); ++i)
		m_vecCam[i]->Render();
}

void CRenderManager::_RenderTool()
{
	for (UINT i = 0; i < m_vecToolCam.size(); ++i) {
		if (!m_vecToolCam[i])
			m_vecToolCam[i]->Render();
	}

	UnRegisterToolCamera();
}

CCamera* CRenderManager::GetMainCamera()
{
	// TODO (Jang) : 어떤 카메라를 가져올지 설정한 카메로 가져오기
	if (E_SceneMode::Play == CSceneManager::GetInstance()->GetSceneMode()) {
		if (m_vecCam.empty())
			return nullptr;
	}
	return m_vecCam[0];
}

CCamera* CRenderManager::GetToolCamera()
{
	if (m_vecToolCam.empty())
		return nullptr;
	return m_vecToolCam[0];
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
		m_pLight2DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iElementCnt);
	}

	vector<TLightInfo> vecLightInfo;
	for (UINT i = 0; i < iLightSize; ++i)
		vecLightInfo.push_back(m_vecLight2D[i]->GetLightInfo());

	m_pLight2DBuffer->SetData(vecLightInfo.data(), (UINT)vecLightInfo.size());
	m_pLight2DBuffer->UpdateData(REGISTER_NUM_Light2DBuffer);
}

void CRenderManager::_RenderClear()
{
	m_vecLight2D.clear();
	m_vecTileMap.clear();
}
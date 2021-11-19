#include "pch.h"
#include "CRenderManager.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CSceneManager.h"
#include "CStructuredBuffer.h"
#include "CCamera.h"

#include "CResourceManager.h"

CRenderManager::CRenderManager() :
	m_pLight2DBuffer(nullptr),
	m_pPostEffectTargetTex(nullptr)
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

	Vector2 vResolution = CDevice::GetInstance()->GetRenderResolution();
	m_pPostEffectTargetTex = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_PostEffectTargetTexture, (UINT)vResolution.x, (UINT)vResolution.y, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
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
		break;
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
	// In Game Scene의 카메라 기준 렌더링
	for (UINT i = 0; i < m_vecCam.size(); ++i) {

		m_vecCam[i]->_SortObjects();
		m_vecCam[i]->_RenderForward();
		m_vecCam[i]->_RenderParticle();
		m_vecCam[i]->_RenderCollider2D();
		m_vecCam[i]->_RenderPostEffect();
		m_vecCam[i]->_RenderCanvas(); // UI Render
	}
}

void CRenderManager::_RenderTool()
{
	// Tool 기준 렌더링
	for (UINT i = 0; i < m_vecToolCam.size(); ++i) {
		m_vecToolCam[i]->_SortObjects();
		m_vecToolCam[i]->_RenderForward();
		m_vecToolCam[i]->_RenderParticle();
		m_vecToolCam[i]->_RenderCollider2D();
		m_vecToolCam[i]->_RenderPostEffect();
		m_vecToolCam[i]->_RenderCanvas(); // UI Render
	}
}

CCamera* CRenderManager::GetMainCamera()
{
	CCamera* pMainCamera = nullptr;
	// TODO (Jang) : 어떤 카메라를 가져올지 설정한 카메로 가져오기

	if (E_SceneMode::Play == CSceneManager::GetInstance()->GetSceneMode()) {
		for (UINT i = 0; i < m_vecCam.size(); ++i) {
			if (NUM_LAYER_UI == m_vecCam[i]->GetGameObject()->GetLayer())
				continue;
			pMainCamera = m_vecCam[i];
			break;
		}
	}
	else {
		pMainCamera = GetToolCamera();
	}
	return pMainCamera;
}

CCamera* CRenderManager::GetUICamera()
{
	CCamera* pUICam = nullptr;	
	if (E_SceneMode::Play == CSceneManager::GetInstance()->GetSceneMode()) {
		for (UINT i = 0; i < m_vecCam.size(); ++i) {
			if (NUM_LAYER_UI == m_vecCam[i]->GetGameObject()->GetLayer()) {
				pUICam = m_vecCam[i];
				break;
			}
		}
	}
	else {
		pUICam = GetToolUICamera();
	}
	return pUICam;
}

CCamera* CRenderManager::GetToolCamera(const tstring& _strObjName)
{
	CCamera* pToolCam = nullptr;
	for (UINT i = 0; i < m_vecToolCam.size(); ++i) {
		if (_strObjName == m_vecToolCam[i]->GetGameObject()->GetName()) {
			pToolCam = m_vecToolCam[i];
			break;
		}
	}
	return pToolCam;
}

CCamera* CRenderManager::GetToolUICamera(const tstring& _strObjName)
{
	CCamera* pToolCam = nullptr;
	for (UINT i = 0; i < m_vecToolCam.size(); ++i) {
		if (_strObjName == m_vecToolCam[i]->GetGameObject()->GetName()) {
			pToolCam = m_vecToolCam[i];
			break;
		}
	}
	return pToolCam;
}

void CRenderManager::_CopyBackBuffer()
{
	SharedPtr<CTexture> pRenderTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_RTTexture);
	CONTEXT->CopyResource(m_pPostEffectTargetTex->GetTex2D().Get(), pRenderTargetTex->GetTex2D().Get());
}

void CRenderManager::_RenderInit_Light2D()
{
	g_globalConst.iLight2DCount = (int)m_vecLight2D.size();

	// 글로벌 컨스트 버퍼에 올려서 GPU에 넣자.
	static const CConstBuffer* pGlobalCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Global);
	pGlobalCB->SetData(&g_globalConst);
	pGlobalCB->UpdateData();


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
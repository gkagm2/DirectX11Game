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
	// 1. Ÿ�� Ŭ����
	CDevice::GetInstance()->ClearTarget();

	E_SceneMode eSceneMode = CSceneManager::GetInstance()->GetSceneMode();
	switch (eSceneMode) {
	case E_SceneMode::Play:
		_RenderInGame();
		// TODO (Jang) : InGame Camera, Tool Camera�� ������ �� �����
		//_RenderTool();
	case E_SceneMode::Stop:
	case E_SceneMode::Pause:
		_RenderTool();
		break;
	default:
		assert(nullptr);
		break;
	}

	// Ÿ�ϸ� ������
	for (UINT i = 0; i < m_vecTileMap.size(); ++i)
		m_vecTileMap[i]->Render();

	_RenderClear();
	
}

void CRenderManager::_RenderInGame()
{
	// 2. ī�޶� �������� ������
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
	// TODO (Jang) : � ī�޶� �������� ������ ī�޷� ��������
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

	// ����Ʈ ���ۿ� �÷��� GPU�� ����.
	static const CConstBuffer* pLight2DBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Global);
	pLight2DBuffer->SetData(&g_globalConst);
	pLight2DBuffer->UpdateData();


	UINT iLightSize = (UINT)m_vecLight2D.size();
	UINT iElementCnt = m_pLight2DBuffer->GetElementCount();

	// ���� ������ ������ ������ ���� ����ȭ ���� �������� ũ�� ����� �÷���
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
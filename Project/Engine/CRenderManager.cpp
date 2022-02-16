#include "pch.h"
#include "CRenderManager.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CSceneManager.h"
#include "CStructuredBuffer.h"
#include "CCamera.h"

#include "CResourceManager.h"

#include "CEngineDebugScript.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CMeshRenderer.h"

CRenderManager::CRenderManager() :
	m_pLight2DBuffer(nullptr),
	m_pLight3DBuffer(nullptr),
	m_pPostEffectTargetTex(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	// Debug
	Safe_Delete_Vector(m_vecDebugObjPool);
	if (m_pDebugShader.Get())
		delete m_pDebugShader.Get();
	if (m_pDebugMtrl.Get())
		delete m_pDebugMtrl.Get();
}

void CRenderManager::Init()
{
	const UINT iDefaultElementCnt = 5;
	m_pLight2DBuffer = make_unique<CStructuredBuffer>();
	m_pLight2DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iDefaultElementCnt, true);

	m_pLight3DBuffer = make_unique<CStructuredBuffer>();
	m_pLight3DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iDefaultElementCnt, true);

	// Post effect�� Ÿ�� �ؽ��� ���� �� post effect ���͸��� ����
	m_pPostEffectTargetTex = CResourceManager::GetInstance()->GetPostEffectTargetTex();

	// ----------------- Debug Objects ---------------------
	//				Debug ������Ʈ ���� �ʱ�ȭ

	// Shader, Material �ʱ�ȭ

	m_pDebugShader = new CGraphicsShader(E_RenderTimePoint::Forward);
	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	strPath = STR_FILE_PATH_DebugSphereShader;

	m_pDebugShader->CreateVertexShader(STR_FILE_PATH_DebugSphereShader, STR_FUNC_NAME_VTXDebug3D);
	m_pDebugShader->CreatePixelShader(STR_FILE_PATH_DebugSphereShader, STR_FUNC_NAME_PIXDebug3D);

	// Rasterizer
	m_pDebugShader->SetRasterizerState(E_RasterizerState::Wireframe);
	m_pDebugShader->SetBlendState(E_BlendState::Default);

	// Shader Param
	m_pDebugShader->AddShaderParam(TShaderParam{ E_ShaderParam::Vector4_0, _T("Color") });
	
	m_pDebugMtrl= new CMaterial(true);
	m_pDebugMtrl->SetName(_T("DebugMaterial"));
	m_pDebugMtrl->SetShader(m_pDebugShader);

	// Debug GameObject �ʱ�ȭ
	constexpr int iDebugObjCnt = 10;
	CGameObject* pNewObj = nullptr;
	for (int i = 0; i < iDebugObjCnt; ++i) {
		if (nullptr == pNewObj)
			pNewObj = _CreateDebugGameObject();
		else 
			pNewObj = pNewObj->Clone();
		m_vecDebugObjPool.push_back(pNewObj);
	}
}

void CRenderManager::Update()
{
#ifdef _DEBUG
	list<CGameObject*>::iterator iter = m_listDebugObj.begin();
	for (; iter != m_listDebugObj.end(); ++iter)
		(*iter)->Update();

	iter = m_listDebugObj.begin();
	for (; iter != m_listDebugObj.end(); ++iter)
		(*iter)->FinalUpdate();
#endif
}

void CRenderManager::Render()
{
	_UpdateData_Light2D();
	_UpdateData_Light3D();
	_Update_GlobalData();
	// Render
	// 1. Ÿ�� Ŭ����
	CDevice::GetInstance()->ClearTarget();

	E_SceneMode eSceneMode = CSceneManager::GetInstance()->GetSceneMode();
	switch (eSceneMode) {
	case E_SceneMode::Play:
		_RenderInGame();
		// TODO (Jang) : InGame Camera, Tool Camera�� ������ �� �����
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
#ifdef _DEBUG
	_RenderDebug();
#endif

	_RenderClear();
}

void CRenderManager::_RenderInGame()
{
	// In Game Scene�� ī�޶� ���� ������
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
	// Tool ���� ������
	for (UINT i = 0; i < m_vecToolCam.size(); ++i) {
		m_vecToolCam[i]->_SortObjects();
		m_vecToolCam[i]->_RenderForward();
		m_vecToolCam[i]->_RenderParticle();
		m_vecToolCam[i]->_RenderCollider2D();
		m_vecToolCam[i]->_RenderPostEffect();
		m_vecToolCam[i]->_RenderCanvas(); // UI Render
	}
}

void CRenderManager::_RenderDebug()
{
	list<CGameObject*>::iterator iter = m_listDebugObj.begin();
	while (iter != m_listDebugObj.end()) {
		CEngineDebugScript* pDebugScript = (*iter)->GetComponentScript< CEngineDebugScript>();
		if (pDebugScript) {
			if (false == pDebugScript->IsUsed())
				iter = m_listDebugObj.erase(iter);
			else {
				Vector4 vColor = pDebugScript->GetColor();
				(*iter)->MeshRenderer()->GetSharedMaterial()->SetData(E_ShaderParam::Vector4_0, &vColor);
				(*iter)->Render();
				++iter;
			}
		}
		else {
			assert(nullptr);
			++iter;
		}
	}
}

CCamera* CRenderManager::GetMainCamera()
{
	CCamera* pMainCamera = nullptr;
	// TODO (Jang) : � ī�޶� �������� ������ ī�޷� ��������

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

void CRenderManager::RenderDebugSphere(const Vector3& _vWorldPos, const Vector3& _vLocalRot, const Vector3& _vColor, float _fRadius, float _fLifeTime)
{

}

CGameObject* CRenderManager::_GetDebugGameObject()
{
	for (size_t i = 0; i < m_vecDebugObjPool.size(); ++i) {
		CEngineDebugScript* pDebugScript = m_vecDebugObjPool[i]->GetComponentScript<CEngineDebugScript>();
		if (pDebugScript) {
			if (false == pDebugScript->IsUsed()) {
				pDebugScript->SetUsed(true);
				return m_vecDebugObjPool[i];
			}
		}
	}

	CGameObject* pDebugObj = _CreateDebugGameObject();
	m_vecDebugObjPool.push_back(pDebugObj);

	return nullptr;
}

CGameObject* CRenderManager::_CreateDebugGameObject()
{
	CGameObject* pDebugObj = new CGameObject;
	pDebugObj->SetName(_T("Debug GameObject"));
	pDebugObj->AddComponent<CTransform>();
	pDebugObj->AddComponent<CMeshRenderer>();

	CMeshRenderer* pMeshRenderer = pDebugObj->GetComponent<CMeshRenderer>();

	pMeshRenderer->SetMaterial(m_pDebugMtrl);
	return pDebugObj;
}

void CRenderManager::_CopyBackBufferToPostEffectBuffer()
{
	SharedPtr<CTexture> pRenderTargetTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_RTTexture);
	CONTEXT->CopyResource(m_pPostEffectTargetTex->GetTex2D().Get(), pRenderTargetTex->GetTex2D().Get());
}

void CRenderManager::_UpdateData_Light2D()
{
	UINT iLightSize = (UINT)m_vecLight2D.size();
	UINT iElementCnt = m_pLight2DBuffer->GetElementCount();

	if (0 == iLightSize)
		CTexture::Clear(REGISTER_NUM_Light2DBuffer);
	else {
		// ���� ������ ������ ������ ���� ����ȭ ���� �������� ũ�� ����� �÷���
		if (m_pLight2DBuffer->GetElementCount() < iLightSize) {
			m_pLight2DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iLightSize, true);
		}

		vector<TLightInfo> vecLightInfo;
		for (UINT i = 0; i < iLightSize; ++i)
			vecLightInfo.push_back(m_vecLight2D[i]->GetLightInfo());

		m_pLight2DBuffer->SetData(vecLightInfo.data(), sizeof(TLightInfo) * (UINT)vecLightInfo.size());
		m_pLight2DBuffer->UpdateData(REGISTER_NUM_Light2DBuffer);
	}
	g_globalConst.iLight2DCount = (int)m_vecLight2D.size();
}

void CRenderManager::_UpdateData_Light3D()
{
	UINT iLightSize = (UINT)m_vecLight3D.size();
	UINT iElementCnt = m_pLight3DBuffer->GetElementCount();
	if (0 == iLightSize)
		CTexture::Clear(REGISTER_NUM_Light3DBuffer);
	else {
		if (m_pLight3DBuffer->GetElementCount() < iLightSize) {
			m_pLight3DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iLightSize, true);
		}

		vector<TLightInfo> vecLIghtInfo;
		for (UINT i = 0; i < iLightSize; ++i)
			vecLIghtInfo.push_back(m_vecLight3D[i]->GetLightInfo());

		m_pLight3DBuffer->SetData(vecLIghtInfo.data(), sizeof(TLightInfo) * (UINT)vecLIghtInfo.size());
		m_pLight3DBuffer->UpdateData(REGISTER_NUM_Light3DBuffer);
	}
	g_globalConst.iLight3DCount = (int)m_vecLight3D.size();
}

void CRenderManager::_Update_GlobalData()
{
	// �۷ι� ����Ʈ ���ۿ� �÷��� GPU�� ����.
	static const CConstBuffer* pGlobalCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Global);
	pGlobalCB->SetData(&g_globalConst);
	pGlobalCB->UpdateData();
}

void CRenderManager::_RenderClear()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}
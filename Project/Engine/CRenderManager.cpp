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
#include "CMRT.h"

CRenderManager::CRenderManager() :
	m_pLight2DBuffer(nullptr),
	m_pLight3DBuffer(nullptr),
	m_pPostEffectTargetTex(nullptr),
	m_arrMRT{}
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
	Safe_Delete_Array(m_arrMRT);
}

void CRenderManager::Init()
{
	const UINT iDefaultElementCnt = 5;
	m_pLight2DBuffer = make_unique<CStructuredBuffer>();
	m_pLight2DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iDefaultElementCnt, true);

	m_pLight3DBuffer = make_unique<CStructuredBuffer>();
	m_pLight3DBuffer->Create(E_StructuredBufferType::ReadOnly, sizeof(TLightInfo), iDefaultElementCnt, true);

	// Post effect�� Ÿ�� �ؽ��� ���� �� post effect ���͸��� ����
	Vector2 vResolution = CDevice::GetInstance()->GetRenderResolution();
	m_pPostEffectTargetTex = CResourceManager::GetInstance()->GetPostEffectTargetTex();
		/*m_pPostEffectTargetTex  = CResourceManager::GetInstance()->CreateTexture(
			STR_ResourceKey_postEffectTargetTexture_RenderMgr, 
			(UINT)vResolution.x, (UINT)vResolution.y, 
			DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);*/


	// --------------- Multiple Render Targers --------------
	_CreateMultpleRenderTargets();

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
	for (UINT i = 0; i < (UINT)E_MRTType::End; ++i) {
		if (m_arrMRT[i])
			m_arrMRT[i]->Clear();
	}

	_UpdateData_Light2D();
	_UpdateData_Light3D();
	_Update_GlobalData();

	// FIXED (Jang) : ��������
	// Render
	// 1. Ÿ�� Ŭ����
	//CDevice::GetInstance()->ClearTarget();

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

		GetMultipleRenderTargets(E_MRTType::Deferred)->UpdateData();
		m_vecCam[i]->_RenderDeferred();

		GetMultipleRenderTargets(E_MRTType::SwapChain)->UpdateData();
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

		// Deferred ��ü ������ �׸���
		GetMultipleRenderTargets(E_MRTType::Deferred)->UpdateData();
		m_vecToolCam[i]->_RenderDeferred();

		// Light Render 
		GetMultipleRenderTargets(E_MRTType::Light)->UpdateData();
		for (size_t i = 0; i < m_vecLight2D.size(); ++i)
			m_vecLight2D[i]->Render();
		for (size_t i = 0; i < m_vecLight3D.size(); ++i)
			m_vecLight3D[i]->Render();

		// Deferred�� �׷��� ������ Swapchain Target���� �ű�
		SharedPtr<CMesh> pRectMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
		assert(pRectMesh.Get());
		SharedPtr<CMaterial> pMergeMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_MergeMtrl);
		assert(pMergeMtrl.Get());
		pMergeMtrl->UpdateData();
		pRectMesh->Render();
		pMergeMtrl->Clear();

		GetMultipleRenderTargets(E_MRTType::SwapChain)->UpdateData();
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

void CRenderManager::_CreateMultpleRenderTargets()
{
	Vector2 vResolution = CDevice::GetInstance()->GetRenderResolution();

	// SwapChain MRT
	{
		SharedPtr<CTexture> arrTex[MAX_RENDER_TARGET_TEX_CNT] = {
			{CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_RTTexture)},
		};
		Vector4 arrClearColor[MAX_RENDER_TARGET_TEX_CNT] = {
			Vector4{0.4f, 0.4f, 0.4f, 1.f},
		};
		SharedPtr<CTexture> pDSTex = CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_DSTexture);
		 
		m_arrMRT[(UINT)E_MRTType::SwapChain] = new CMRT(arrTex, arrClearColor, 1, pDSTex, false);
	}

	// Deferred MRT
	{
		SharedPtr<CTexture> arrTex[MAX_RENDER_TARGET_TEX_CNT] = {};
		Vector4 arrClearColor[MAX_RENDER_TARGET_TEX_CNT] = {
			Vector4::Zero,
			Vector4::Zero,
			Vector4::Zero,
			Vector4::Zero,
		};

		UINT bindFlag = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		arrTex[0] = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_Deferred_ColorTargetTex, 
			(UINT)vResolution.x, (UINT)vResolution.y, 
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 
			bindFlag);

		arrTex[1] = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_Deferred_NormalTargetTex, 
			(UINT)vResolution.x, (UINT)vResolution.y, 
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 
			bindFlag);

		arrTex[2] = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_Deferred_PositionTargetTex, 
			(UINT)vResolution.x, (UINT)vResolution.y, 
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 
			bindFlag);

		arrTex[3] = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_Deferred_DataTargetTex, 
			(UINT)vResolution.x, (UINT)vResolution.y, 
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 
			bindFlag);

		m_arrMRT[(UINT)E_MRTType::Deferred] = new CMRT(arrTex, arrClearColor, 4, nullptr, true);

		// Directional Light Shader �� �������ڷ� ���۵� Ÿ�� �ؽ��ĵ��� ����
		SharedPtr<CMaterial> pDirLightMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_DirectionLightMtrl);
		pDirLightMtrl->SetData(E_ShaderParam::Texture_0, CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_NormalTargetTex).Get());
		pDirLightMtrl->SetData(E_ShaderParam::Texture_1, CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_PositionTargetTex).Get());
	}

	// Light MRT
	{
		SharedPtr<CTexture> arrTex[MAX_RENDER_TARGET_TEX_CNT] = {};
		Vector4 arrClearColor[MAX_RENDER_TARGET_TEX_CNT] = {
			Vector4::Zero,
			Vector4::Zero,
			Vector4::Zero,
		};

		UINT bindFlag = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		arrTex[0] = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_DiffuseTargetTex,
			(UINT)vResolution.x, (UINT)vResolution.y,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			bindFlag);

		arrTex[1] = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_SpecularTargetTex,
			(UINT)vResolution.x, (UINT)vResolution.y,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			bindFlag);

		arrTex[2] = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_ShadowTargetTex,
			(UINT)vResolution.x, (UINT)vResolution.y,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			bindFlag);

		m_arrMRT[(UINT)E_MRTType::Light] = new CMRT(arrTex, arrClearColor, 3, nullptr, false);

		// Merge shader�� �������ڷ� ���۵� Ÿ�� �ؽ��ĵ��� ����.
		SharedPtr<CMaterial> pMergeMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_MergeMtrl);
		pMergeMtrl->SetData(E_ShaderParam::Texture_0, CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_ColorTargetTex).Get());
		pMergeMtrl->SetData(E_ShaderParam::Texture_1, CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_NormalTargetTex).Get());
		pMergeMtrl->SetData(E_ShaderParam::Texture_2, CResourceManager::GetInstance()->FindRes<CTexture>(STR_ResourceKey_Deferred_PositionTargetTex).Get());
	}
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
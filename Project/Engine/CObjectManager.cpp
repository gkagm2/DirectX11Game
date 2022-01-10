#include "pch.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CRenderManager.h"
#include "CScene.h"
#include "CGameObject.h"

#include "CTransform.h"
#include "CRectTransform.h"
#include "CTextUI.h"
#include "CMeshRenderer.h"



UINT g_iMtrlID = 0;
UINT g_iEmptyGameObjectID = 0;
UINT g_iEmpty2DCameraGameObjectID = 0;
UINT g_iEmptyRect2DGameObjectID = 0;
UINT g_iEmptyParticleSystemGameObjectID = 0;
UINT g_iEmptyLight2DID = 0;


UINT g_iTextUIGameObjectID = 0;
UINT g_iButtonUIGameObjectID = 0;
UINT g_iImageUIGameObjectID = 0;

CObjectManager::CObjectManager(){}
CObjectManager::~CObjectManager(){}



tstring CObjectManager::_CreateObjectName(const tstring& _strObjDefaultName, UINT& id)
{
	constexpr int iBuffSize = 255;
	TCHAR szBuffer[iBuffSize] = _T("");

	// 고유 이름값 생성
	while (true) {
		_stprintf_s(szBuffer, iBuffSize, _T("%s%d"), _strObjDefaultName.c_str(), id++);
		CGameObject* pObj = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(szBuffer);
		if (nullptr == pObj)
			break;
	}
	return szBuffer;
}

CMaterial* CObjectManager::CreateEmptyMaterial()
{
	constexpr int iBuffSize = 255;
	TCHAR szBuff[iBuffSize] = _T("");

	tstring strRelativePath = STR_FILE_PATH_Material;
	tstring strExtension = STR_EXTENSION_Mtrl;
	// 고유 이름값 생성
	while (true) {

		_stprintf_s(szBuff, iBuffSize, _T("%sMaterial %d%s"), strRelativePath.c_str(), g_iMtrlID++, strExtension.c_str());
		CMaterial* pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(szBuff).Get();
		if (nullptr == pMtrl)
			break;
	}

	tstring strKey = szBuff;
	strRelativePath = szBuff;
	CMaterial* pNewMtrl = new CMaterial(false); // Create Material 
	pNewMtrl->SetKey(strKey);
	pNewMtrl->SetRelativePath(strRelativePath);

	CResourceManager::GetInstance()->AddRes<CMaterial>(strKey, pNewMtrl);

	if (!pNewMtrl->IsDefaultMaterial() && E_SceneMode::Stop == CSceneManager::GetInstance()->GetSceneMode())
		pNewMtrl->Save(strRelativePath);
	return nullptr;
}

CGameObject* CObjectManager::CreateEmptyGameObject(UINT _iLayer)
{
	tstring strObjName = _CreateObjectName(_T("GameObject"), g_iEmptyGameObjectID);

	// 새 게임 오브젝트 생성
	CGameObject* pNewGameObject = new CGameObject;
	pNewGameObject->SetName(strObjName);
	pNewGameObject->AddComponent<CTransform>();

	// Tool Camera가 바라보고 있는 위치에 생성
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();


	Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
	if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType())
		vWorldPos.z = 0.f;
	pNewGameObject->Transform()->SetLocalPosition(vWorldPos);
	CObject::CreateGameObjectEvn(pNewGameObject, _iLayer);

	return pNewGameObject;
}

CGameObject* CObjectManager::CreateCamera2DGameObject(UINT _iLayer)
{
	tstring strObjName = _CreateObjectName(_T("2D Camera"), g_iEmpty2DCameraGameObjectID);

	// 새 게임 오브젝트 생성
	CGameObject* pNewGameObject = new CGameObject;
	pNewGameObject->SetName(strObjName);
	pNewGameObject->AddComponent<CTransform>();
	pNewGameObject->AddComponent<CCamera>();
	pNewGameObject->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pNewGameObject->Camera()->SetLayerCheckAll();
	pNewGameObject->Camera()->SetLayerCheck(NUM_LAYER_UI, false);

	// Tool Camera가 바라보고 있는 위치에 생성
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
	vWorldPos.z = -500.f;
	pNewGameObject->Transform()->SetLocalPosition(vWorldPos);
	CObject::CreateGameObjectEvn(pNewGameObject, _iLayer);

	return pNewGameObject;
}

CGameObject* CObjectManager::CreateParticleSystemGameObject(UINT _iLayer)
{
	tstring strObjName = _CreateObjectName(_T("Particle"), g_iEmptyParticleSystemGameObjectID);

	CGameObject* pNewGameObject = new CGameObject;
	pNewGameObject->SetName(strObjName);
	pNewGameObject->AddComponent<CTransform>();
	pNewGameObject->AddComponent<CParticleSystem>();
	CObject::CreateGameObjectEvn(pNewGameObject, _iLayer);

	return pNewGameObject;
}

CGameObject* CObjectManager::CreateLight2D(UINT _iLayer)
{
	tstring strObjName = _CreateObjectName(_T("Light2D"), g_iEmptyLight2DID);

	CGameObject* pNewGameObject = new CGameObject;
	pNewGameObject->SetName(strObjName);
	pNewGameObject->AddComponent<CTransform>();
	pNewGameObject->AddComponent<CLight2D>();
	CObject::CreateGameObjectEvn(pNewGameObject, _iLayer);

	return pNewGameObject;
}

CGameObject* CObjectManager::Create2DRectGameObject(UINT _iLayer)
{
	tstring strObjName = _CreateObjectName(_T("Rect GameObject"), g_iEmptyRect2DGameObjectID);

	// 새 게임 오브젝트 생성
	CGameObject* pNewGameObject = new CGameObject;
	pNewGameObject->SetName(strObjName);
	pNewGameObject->AddComponent<CTransform>();
	pNewGameObject->AddComponent<CMeshRenderer>();
	pNewGameObject->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdLight2DMtrl));

	// Tool Camera가 바라보고 있는 위치에 생성
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
	if (E_ProjectionType::Orthographic == pToolCam->GetProjectionType())
		vWorldPos.z = 0.f;
	pNewGameObject->Transform()->SetLocalPosition(vWorldPos);
	CObject::CreateGameObjectEvn(pNewGameObject, _iLayer);

	return pNewGameObject;
}

CGameObject* CObjectManager::_CreateUIGameObject()
{
	CGameObject* pUIGameObject = new CGameObject;
	pUIGameObject->AddComponent<CRectTransform>();
	pUIGameObject->AddComponent<CCanvasRenderer>();
	return pUIGameObject;
}

CGameObject* CObjectManager::_CreateCanvas()
{
	CGameObject* pUICanvas = _CreateUIGameObject();
	pUICanvas->SetName(STR_OBJ_NAME_UICanvas);
	pUICanvas->RectTransform()->SetLocalPosition(Vector3(9999.f, 9999.f, 0.f));
	CObject::CreateGameObjectEvn(pUICanvas, NUM_LAYER_UI);
	return pUICanvas;
}

CGameObject* CObjectManager::_CreateDefaultUICamera()
{
	tstring strObjName = STR_OBJ_NAME_UICamera;

	// 새 게임 오브젝트 생성
	CGameObject* pUICameraObj = new CGameObject;
	pUICameraObj->SetName(strObjName);
	pUICameraObj->AddComponent<CTransform>();
	pUICameraObj->AddComponent<CCamera>();
	pUICameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pUICameraObj->Camera()->SetSize(1.f);
	pUICameraObj->Camera()->SetLayerCheckAllUnActive();
	pUICameraObj->Camera()->SetLayerCheck(NUM_LAYER_UI, true);
	TClippingPlanes tCp;
	tCp.fFar = 1000.f;
	tCp.fNear = 0.f;
	pUICameraObj->Camera()->SetClippingPlanes(tCp);

	// Tool Camera가 바라보고 있는 위치에 생성
	CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	Vector3 vWorldPos = pToolCam->Transform()->GetPosition();
	pUICameraObj->Transform()->SetLocalPosition(DEFAULT_UI_CAM_POS);
	CObject::CreateGameObjectEvn(pUICameraObj, NUM_LAYER_UI);
	return pUICameraObj;
}

CGameObject* CObjectManager::CreateTextUI()
{
	tstring strObjName = _CreateObjectName(_T("TextUI"), g_iTextUIGameObjectID);

	// 새 게임 오브젝트 생성
	CGameObject* pTextUIObj = _CreateUIGameObject();
	pTextUIObj->SetName(strObjName);
	pTextUIObj->AddComponent<CTextUI>();
	pTextUIObj->TextUI()->SetText(_T("Text"));

	// 자식 오브젝트로 넣기

	CGameObject* pUICamera = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(STR_OBJ_NAME_UICamera, NUM_LAYER_UI);

	// UI 카메라가 없으면 새로 생성한다.
	if (!pUICamera)
		pUICamera = _CreateDefaultUICamera();

	CGameObject* pUICanvas = FIND_GameObject_Layer(STR_OBJ_NAME_UICanvas, NUM_LAYER_UI);
	if (!pUICanvas)
		pUICanvas = _CreateCanvas();
	CObject::CreateGameObjectEvn(pTextUIObj, NUM_LAYER_UI);
	CObject::AddChildGameObjectEvn(pUICanvas, pTextUIObj);

	return pTextUIObj;
}

CGameObject* CObjectManager::CreateImageUI()
{
	tstring strObjName = _CreateObjectName(_T("ImageUI"), g_iImageUIGameObjectID);

	// 새 게임 오브젝트 생성
	CGameObject* pImageUI = _CreateUIGameObject();
	pImageUI->SetName(strObjName);
	pImageUI->AddComponent<CImageUI>();

	// 자식 오브젝트로 넣기

	CGameObject* pUICamera = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(STR_OBJ_NAME_UICamera, NUM_LAYER_UI);

	// UI 카메라가 없으면 새로 생성한다.
	if (!pUICamera)
		pUICamera = _CreateDefaultUICamera();

	CGameObject* pUICanvas = FIND_GameObject_Layer(STR_OBJ_NAME_UICanvas, NUM_LAYER_UI);
	if (!pUICanvas)
		pUICanvas = _CreateCanvas();
	CObject::CreateGameObjectEvn(pImageUI, NUM_LAYER_UI);
	CObject::AddChildGameObjectEvn(pUICanvas, pImageUI);

	return pImageUI;
}

CGameObject* CObjectManager::CreateButtonUI()
{
	tstring strObjName = _CreateObjectName(_T("ButtonUI"), g_iButtonUIGameObjectID);

	// 새 게임 오브젝트 생성
	CGameObject* pButtonUIObj = _CreateUIGameObject();
	pButtonUIObj->SetName(strObjName);
	pButtonUIObj->AddComponent<CButtonUI>();

	// 자식 오브젝트로 넣기

	CGameObject* pUICamera = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(STR_OBJ_NAME_UICamera, NUM_LAYER_UI);

	// UI 카메라가 없으면 새로 생성한다.
	if (!pUICamera)
		pUICamera = _CreateDefaultUICamera();

	CGameObject* pUICanvas = FIND_GameObject_Layer(STR_OBJ_NAME_UICanvas, NUM_LAYER_UI);
	if (!pUICanvas)
		pUICanvas = _CreateCanvas();
	CObject::CreateGameObjectEvn(pButtonUIObj, NUM_LAYER_UI);
	CObject::AddChildGameObjectEvn(pUICanvas, pButtonUIObj);

	return pButtonUIObj;
}
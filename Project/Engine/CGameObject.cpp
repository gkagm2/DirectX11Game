#include "pch.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"


// Component
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CCamera.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CAnimator2D.h"
#include "CLight2D.h"
#include "CTileMap.h"
#include "CParticleSystem.h"
#include "CRigidbody2D.h"
#include "CAudioSource.h"
#include "CRectTransform.h"
#include "CCanvasRenderer.h"
#include "CSpriteRenderer.h"

#include "CTextUI.h"
#include "CButtonUI.h"
#include "CImageUI.h"

#include "CScript.h"


#include "CResourceManager.h"
#include "CPrefab.h"

CGameObject::CGameObject() :
	m_arrComponent{},
	m_pParentObj(nullptr),
	m_iLayer(MAX_SIZE_LAYER),
	m_iTag(0),
	m_bDead(false),
	m_bActive(true)
{
}

CGameObject::CGameObject(const CGameObject& _origin) :
	CObject(_origin),
	m_arrComponent{},
	m_pParentObj(nullptr),
	m_iLayer(MAX_SIZE_LAYER),
	m_iTag(_origin.m_iTag),
	m_bDead(false),
	m_bActive(_origin.m_bActive)
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != _origin.m_arrComponent[i])
			AddComponent((CComponent*)_origin.m_arrComponent[i]->Clone());
	}

	for (UINT i = 0; i < _origin.m_vecScript.size(); ++i)
		AddComponent(_origin.m_vecScript[i]->Clone());

	for (UINT i = 0; i < _origin.m_vecChildObj.size(); ++i)
		_AddChildGameObject(_origin.m_vecChildObj[i]->Clone(),true);
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(m_arrComponent);
	Safe_Delete_Vector(m_vecChildObj);
	Safe_Delete_Vector(m_vecScript);
}

void CGameObject::Awake()
{
	if (IsDead() || !IsActive())
		return;

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->Awake();

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i]) {
			if (m_arrComponent[i]->IsActive()) {
				m_arrComponent[i]->Awake();
				m_arrComponent[i]->OnEnable();
			}
				
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i) {
		if (m_vecScript[i]->IsActive()) {
			m_vecScript[i]->Awake();
			m_vecScript[i]->OnEnable();
		}
	}
}

void CGameObject::Start()
{
	if (IsDead() || !IsActive())
		return;

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->Start();

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i]) {
			if(m_arrComponent[i]->IsActive())
				m_arrComponent[i]->Start();
		}
	}
	for (UINT i = 0; i < m_vecScript.size(); ++i) {
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->Start();
	}
}

void CGameObject::PrevUpdate()
{
	if (IsDead() || !IsActive())
		return;

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->PrevUpdate();

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i]) {
			if (m_arrComponent[i]->IsActive())
				m_arrComponent[i]->PrevUpdate();
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i) {
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->PrevUpdate();
	}
}

void CGameObject::Update()
{
	if (IsDead() || !IsActive())
		return;

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->Update();

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i]) {
			if (m_arrComponent[i]->IsActive())
				m_arrComponent[i]->Update();
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i) {
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->Update();
	}
}

void CGameObject::LateUpdate()
{
	if (IsDead() || !IsActive())
		return;

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->LateUpdate();

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i]) {
			if (m_arrComponent[i]->IsActive())
				m_arrComponent[i]->LateUpdate();
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i) {
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->LateUpdate();
	}
}

void CGameObject::FinalUpdate()
{
	_RegisterLayer(); // 레이어 등록

	if (IsDead())
		return;
	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->FinalUpdate();

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i]) {
			if(m_arrComponent[i]->IsActive())
				m_arrComponent[i]->FinalUpdate();
		}
	}
}

void CGameObject::Render()
{
	if (MeshRenderer() && MeshRenderer()->IsActive())		// 메쉬 렌더링
		MeshRenderer()->Render();
	else if (SpriteRenderer() && SpriteRenderer()->IsActive())
		SpriteRenderer()->Render();
	else if (CanvasRenderer() && CanvasRenderer()->IsActive())
		CanvasRenderer()->Render();
	else if (TileMap() && TileMap()->IsActive())			// 타일맵 렌더링
		TileMap()->Render();
	else if (ParticleSystem() && ParticleSystem()->IsActive())	// 파티클 시스템 렌더링
		ParticleSystem()->Render();

	if (Collider2D() && Collider2D()->IsActive())		// 충돌체 렌더링
		Collider2D()->Render();
}

void CGameObject::RegisterAsPrefab(const tstring& _strName)
{
	tstring strName = _strName;
	if (strName.empty()) // 비어있으면 오브젝트 이름을 프리펩 이름으로 설정
		strName = GetName();

	SharedPtr<CPrefab> pPrefab = new CPrefab(this->Clone());

	CResourceManager::GetInstance()->AddRes<CPrefab>(strName, pPrefab.Get());
	tstring strRelativePath = STR_FILE_PATH_Prefab + strName + STR_EXTENSION_Prefab;
	pPrefab->SetRelativePath(strRelativePath);
	pPrefab->Save(strRelativePath);
}

void CGameObject::SetTag(UINT _iTag, bool _bChangeChilds)
{
	m_iTag = _iTag;
	if (_bChangeChilds) {
		vector<CGameObject*>& pChilds = GetChildsObject();
		for (size_t i = 0; i < pChilds.size(); ++i)
			pChilds[i]->SetTag(_iTag, _bChangeChilds);
	}
}

void CGameObject::SetActive(bool _bActive, bool _bWithChilds)
{
	bool isFixed = false;
	if (m_bActive != _bActive)
		isFixed = true;

	if (isFixed) {
		if (_bWithChilds) {
			if (_bActive) { // 오브젝트를 enable하게 함.
				for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
					if (m_arrComponent[i])
						if (m_arrComponent[i]->IsActive()) {
							if (_bActive)
								m_arrComponent[i]->OnEnable();
							else
								m_arrComponent[i]->OnDisable();
						}
				}
				for (size_t i = 0; i < m_vecScript.size(); ++i) {
					if (m_vecScript[i]->IsActive()) {
						if (_bActive)
							m_vecScript[i]->OnEnable();
						else
							m_vecScript[i]->OnDisable();
					}
				}
				const vector<CGameObject*>& vecChilds = GetChildsObject();
				for (size_t i = 0; i < vecChilds.size(); ++i)
					vecChilds[i]->SetActive(_bActive, _bWithChilds);

			}
			else { // 오브젝트를 disable하게 함.
				list<CGameObject*> que;
				list<CGameObject*> stk;
				que.push_back(this);
				while (!que.empty()) {
					CGameObject* pObj = que.front();
					stk.push_back(pObj);
					que.pop_front();

					const vector<CGameObject*>& vecChilds = pObj->GetChildsObject();
					for (size_t i = 0; i < vecChilds.size(); ++i)
						que.push_back(vecChilds[i]);
				}
				while (!stk.empty()) {
					CGameObject* pObj = stk.back();
					stk.pop_back();
					pObj->SetActive(_bActive);
				}
			}
		}
		else {
			for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
				if (m_arrComponent[i])
					if (m_arrComponent[i]->IsActive()) {
						if (_bActive)
							m_arrComponent[i]->OnEnable();
						else
							m_arrComponent[i]->OnDisable();
					}
			}
			for (size_t i = 0; i < m_vecScript.size(); ++i) {
				if (m_vecScript[i]->IsActive()) {
					if (_bActive)
						m_vecScript[i]->OnEnable();
					else
						m_vecScript[i]->OnDisable();
				}
			}
		}
		m_bActive = _bActive;
	}
	else {
		if (_bWithChilds) {
			if (_bActive) { // 오브젝트를 enable하게 함.
				for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
					if (m_arrComponent[i])
						if (m_arrComponent[i]->IsActive()) {
							if (_bActive)
								m_arrComponent[i]->OnEnable();
							else
								m_arrComponent[i]->OnDisable();
						}
				}
				for (size_t i = 0; i < m_vecScript.size(); ++i) {
					if (m_vecScript[i]->IsActive()) {
						if (_bActive)
							m_vecScript[i]->OnEnable();
						else
							m_vecScript[i]->OnDisable();
					}
				}
				const vector<CGameObject*>& vecChilds = GetChildsObject();
				for (size_t i = 0; i < vecChilds.size(); ++i)
					vecChilds[i]->SetActive(_bActive, _bWithChilds);

			}
			else { // 오브젝트를 disable하게 함.
				list<CGameObject*> que;
				list<CGameObject*> stk;
				que.push_back(this);
				while (!que.empty()) {
					CGameObject* pObj = que.front();
					stk.push_back(pObj);
					que.pop_front();

					const vector<CGameObject*>& vecChilds = pObj->GetChildsObject();
					for (size_t i = 0; i < vecChilds.size(); ++i)
						que.push_back(vecChilds[i]);
				}
				while (!stk.empty()) {
					CGameObject* pObj = stk.back();
					stk.pop_back();
					pObj->SetActive(_bActive);
				}
			}
		}
	}
}

CGameObject* CGameObject::FindGameObjectInChilds(const tstring& _strObjName)
{
	CGameObject* pFindObject = nullptr;
	list<CGameObject*> que;
	const vector<CGameObject*>& vecChilds = GetChildsObject();
	for (UINT i = 0; i < vecChilds.size(); ++i)
		que.push_back(vecChilds[i]);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();
		if (_strObjName == pObj->GetName()) {
			pFindObject = pObj;
			break;
		}
		const vector<CGameObject*>& childs = pObj->GetChildsObject();
		for (UINT i = 0; i < childs.size(); ++i)
			que.push_back(childs[i]);
	}

	return pFindObject;
}

CGameObject* CGameObject::FindGameObjectInParent(const tstring& _strObjName)
{
	CGameObject* pParent = GetParentObject();
	while (nullptr != pParent) {
		if (_strObjName == pParent->GetName())
			break;
		pParent = pParent->GetParentObject();
	}
	return pParent;
}

CGameObject* CGameObject::GetRootObject()
{
	CGameObject* pRootObj = this;
	while (pRootObj->GetParentObject())
		pRootObj = pRootObj->GetParentObject();

	return pRootObj;
}

void CGameObject::_SetDead()
{
	list<CGameObject*> que;
	que.push_back(this);

	while (!que.empty()) {
		CGameObject* pObj = que.front();
		que.pop_front();

		pObj->m_bDead = true;

		for (UINT i = 0; i < pObj->m_vecChildObj.size(); ++i) {
			que.push_back(pObj->m_vecChildObj[i]);
		}
	}
}

void CGameObject::_AddChildGameObject(CGameObject* _pChildObj, bool _IsSaveLoad)
{
	// 자식 오브젝트가 Ancestor 오브젝트면 안됨
	if (_IsAncestorGameObject(_pChildObj))
		return;
	
	// 자식 오브젝트가 최상위 오브젝트인 경우
	if (nullptr == _pChildObj->GetParentObject() && MAX_SIZE_LAYER != _pChildObj->GetLayer()) {
		if (_pChildObj->GetLayer() == GetLayer()) {
			// 레이어의 Root들의 오브젝트를 저장하는걸 해제
			CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(_pChildObj->GetLayer());
			pLayer->_UnRegisterInRootGameObject(_pChildObj);
		}
	}

	// 자식 오브젝트가 부모가 있으면 해제
	_pChildObj->_UnlinkParentGameObject(_IsSaveLoad);

	// 부모와 자식 연결
	_pChildObj->m_pParentObj = this;
	m_vecChildObj.push_back(_pChildObj);


	// 소속된 레이어가 없으면 부모 오브젝트의 레이어로 설정.
	if (MAX_SIZE_LAYER == _pChildObj->GetLayer())
		_pChildObj->_SetLayer(GetLayer());

	if (false == _IsSaveLoad) {
		if (_pChildObj->Transform())
			_pChildObj->Transform()->_LinkParent();
	}
}

void CGameObject::_RegisterLayer()
{
	CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(m_iLayer);
	pLayer->RegisterGameObject(this);
}

CGameObject* CGameObject::FindGameObjectSameLine(const tstring& _strObjName)
{
	CGameObject* pParent = GetParentObject();
	if (pParent) {
		vector<CGameObject*>& childs = pParent->GetChildsObject();
		for (size_t i = 0; i < childs.size(); ++i) {
			if (childs[i]->GetName() == _strObjName)
				return childs[i];
		}
	}
	else {
		vector<CGameObject*> rootObjs;
		CSceneManager::GetInstance()->GetCurScene()->GetRootGameObjects(rootObjs);
		for (size_t i = 0; i < rootObjs.size(); ++i) {
			if (rootObjs[i]->GetName() == _strObjName)
				return rootObjs[i];
		}
	}
	return nullptr;
}

void CGameObject::_UnlinkParentGameObject(bool _IsSaveLoad)
{
	UINT iLayer = GetLayer();
	if (iLayer != MAX_SIZE_LAYER) {
		CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(GetLayer());
		vector<CGameObject*>& vecRootObjs = pLayer->GetRootGameObjects();
		vector<CGameObject*>::iterator rootObjIter = vecRootObjs.begin();
		for (; rootObjIter != vecRootObjs.end(); ++rootObjIter) {
			if (*rootObjIter == this) {
				vecRootObjs.erase(rootObjIter);
				break;
			}
		}
	}

	if (nullptr == m_pParentObj)
		return;

	vector<CGameObject*>& vecChild = m_pParentObj->_GetChildsObjectRef();
	vector<CGameObject*>::iterator iter = vecChild.begin();

	for (; iter != vecChild.end(); ++iter) {
		if (*iter == this) {
			vecChild.erase(iter);
			break;
		}
	}
	if (true == _IsSaveLoad) {
		m_pParentObj = nullptr;
	}
	else {
		Vector3 vParentScale = Vector3::One;
		Vector3 vParentRotation = Vector3::Zero;
		if (Transform())
			vParentRotation = Transform()->GetRotation();
		if (m_pParentObj->Transform()) {
			vParentScale = m_pParentObj->Transform()->GetScale();
			vParentRotation = m_pParentObj->Transform()->GetRotation();
		}

		m_pParentObj = nullptr;

		if (false == _IsSaveLoad) {
			if (Transform())
				Transform()->_UnlinkParent(vParentScale, vParentRotation);
		}
	}
}

bool CGameObject::_IsAncestorGameObject(CGameObject* _pObj)
{
	CGameObject* pParentObj = m_pParentObj;
	while (pParentObj != nullptr) {
		if (_pObj == pParentObj)
			return true;
		pParentObj = pParentObj->GetParentObject();
	}
	return false;
}

bool CGameObject::_IsOnlyOnePossibleRenderComponent(E_ComponentType _eComponentType)
{
	bool bIsOnlyOneRenderComponent = false;
	for (UINT i = ONLY_ONE_POSSIBLE_RENDERING_START_IDX; i < ONLY_ONE_POSSIBLE_RENDERING_END_IDX; ++i) {
		if (_eComponentType == (E_ComponentType)i) {
			bIsOnlyOneRenderComponent = true;
			break;
		}
	}

	return bIsOnlyOneRenderComponent;
}

CComponent* CGameObject::AddComponent(CComponent* _pComponent)
{
	if (E_ComponentType::Script == _pComponent->GetComponentType()) {
		m_vecScript.push_back((CScript*)_pComponent);
		_pComponent->m_pGameObj = this;
		return _pComponent;
	}

	if (m_arrComponent[(UINT)_pComponent->GetComponentType()])
		return m_arrComponent[(UINT)_pComponent->GetComponentType()];

	// 오직 하나만 렌더링할 수 있는 종류의 컴포넌트인지 체크
	bool bIsOnlyOneRenderComponent = _IsOnlyOnePossibleRenderComponent(_pComponent->GetComponentType());
	if (bIsOnlyOneRenderComponent) {
		for (UINT i = ONLY_ONE_POSSIBLE_RENDERING_START_IDX; i < ONLY_ONE_POSSIBLE_RENDERING_END_IDX; ++i) {
			if (_pComponent->GetComponentType() == (E_ComponentType)i)
				continue;
			if (_IsExistComponent((E_ComponentType)i)) {
				assert(nullptr && _T("이미 렌더링하는 다른 컴포넌트가 존재함"));
				if (_pComponent)
					delete _pComponent;
				return nullptr;
			}
		}
	}
	
	if (E_ComponentType::Rigidbody2D == _pComponent->GetComponentType()) {
		if (nullptr == GetComponent<CCollider2D>())
			AddComponent(new CCollider2D);
	}
	if (E_ComponentType::Rigidbody3D == _pComponent->GetComponentType()) {
		if (nullptr == GetComponent<CCollider3D>())
			AddComponent(new CCollider3D);
	}


	m_arrComponent[(UINT)_pComponent->GetComponentType()] = _pComponent;
	_pComponent->m_pGameObj = this;

	return _pComponent;
}

CComponent* CGameObject::GetComponent(E_ComponentType _eType)
{
	return m_arrComponent[(UINT)_eType];
}

bool CGameObject::SaveToScene(FILE* _pFile)
{
	CObject::SaveToScene(_pFile);
	FWrite(m_bActive, _pFile);

	// 자식 오브젝트일 경우
	if (GetParentObject())
		FWrite(m_iLayer, _pFile);

	FWrite(m_iTag, _pFile);
	
	// 자식 오브젝트
	UINT iChildCount = (UINT)m_vecChildObj.size();
	FWrite(iChildCount, _pFile);

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->SaveToScene(_pFile);

	// 컴포넌트
	UINT iComIdx = 0;
	for (; iComIdx < (UINT)E_ComponentType::End; ++iComIdx) {
		if (nullptr == m_arrComponent[iComIdx])
			continue;

		FWrite(iComIdx, _pFile);
		m_arrComponent[iComIdx]->SaveToScene(_pFile);
	}
	FWrite(iComIdx, _pFile); // 마감
	
	// 스크립트 정보
	UINT iScriptCount = (UINT)m_vecScript.size();
	FWrite(iScriptCount, _pFile);

	for (UINT i = 0; i < m_vecScript.size(); ++i) {
		// SceneManager에 등록된 함수를 사용해서 Script를 저장
		CSceneManager::GetInstance()->m_pSaveScript(m_vecScript[i], _pFile);
	}
	
	return true;
}

bool CGameObject::LoadFromScene(FILE* _pFile, int _iDepth)
{
	CObject::LoadFromScene(_pFile);
	FRead(m_bActive, _pFile);

	// 자식 오브젝트인 경우 Layer 소속 읽기
	if (0 != _iDepth)
		FRead(m_iLayer, _pFile);

	FRead(m_iTag, _pFile);

	// 자식 정보
	++_iDepth;

	UINT iChildCount = 0;
	FRead(iChildCount, _pFile);
	for (UINT i = 0; i < iChildCount; ++i) {
		CGameObject* pChildObj = new CGameObject;
		pChildObj->LoadFromScene(_pFile, _iDepth);
		_AddChildGameObject(pChildObj, true);
	}

	// 컴포넌트 정보
	UINT iComIdx = (UINT)E_ComponentType::End;
	while (true) {
		FRead(iComIdx, _pFile);
		if ((UINT)E_ComponentType::End == iComIdx) // 마감이 나오면 종료
			break;

		CComponent* pComponent = CreateComponent((E_ComponentType)iComIdx);

		pComponent->LoadFromScene(_pFile);
		AddComponent(pComponent);
	}

	// 스크립트 정보
	UINT iScriptCount = 0;
	FRead(iScriptCount, _pFile);
	m_vecScript.clear();
	for (UINT i = 0; i < iScriptCount; ++i) {
		CScript* pScript = CSceneManager::GetInstance()->m_pLoadScript(_pFile);
		if (nullptr == pScript) {
			_tcprintf(_T("[Error] Script Data를 찾을 수 없음"));
		}
		else {
			AddComponent(pScript);
		}
	}
		
	return true;
}

CComponent* CGameObject::CreateComponent(E_ComponentType _eType)
{
	CComponent* pComponent = nullptr;
	switch ((E_ComponentType)_eType) {
	case E_ComponentType::Transform:
		pComponent = new CTransform;
		break;
	case E_ComponentType::MeshRenderer:
		pComponent = new CMeshRenderer;
		break;
	case E_ComponentType::Camera:
		pComponent = new CCamera;
		break;
	case E_ComponentType::Collider2D:
		pComponent = new CCollider2D;
		break;
	case E_ComponentType::Collider3D:
		pComponent = new CCollider3D;
		break;
	case E_ComponentType::Animator2D:
		pComponent = new CAnimator2D;
		break;
	case E_ComponentType::Light2D:
		pComponent = new CLight2D;
		break;
	case E_ComponentType::TileMap:
		pComponent = new CTileMap;
		break;
	case E_ComponentType::ParticleSystem:
		pComponent = new CParticleSystem;
		break;
	case E_ComponentType::Rigidbody2D:
		pComponent = new CRigidbody2D;
		break;
	case E_ComponentType::AudioSource:
		pComponent = new CAudioSource;
		break;
	case E_ComponentType::RectTransform:
		pComponent = new CRectTransform;
		break;
	case E_ComponentType::SpriteRenderer:
		pComponent = new CSpriteRenderer;
		break;
	case E_ComponentType::CanvasRenderer:
		pComponent = new CCanvasRenderer;
		break;
	case E_ComponentType::TextUI:
		pComponent = new CTextUI;
		break;
	case E_ComponentType::ImageUI:
		pComponent = new CImageUI;
		break;
	case E_ComponentType::ButtonUI:
		pComponent = new CButtonUI;
		break;
	default:
		assert(nullptr);
		break;
	}
	return pComponent;
}

void CGameObject::_DestroyComponent(E_ComponentType _eType)
{
	if (m_arrComponent[(UINT)_eType]) {
		delete m_arrComponent[(UINT)_eType];
		m_arrComponent[(UINT)_eType] = nullptr;
	}
}

void CGameObject::_DestroyScript(CScript* pScript)
{
	UINT iScriptType = pScript->GetScriptType();
	vector<CScript*>::iterator iter = m_vecScript.begin();
	
	for (; iter != m_vecScript.end(); ++iter) {
		if(iScriptType == (*iter)->GetScriptType())
			break;
	}
	if (iter != m_vecScript.end()) {
		CScript* pScr = *iter;
		m_vecScript.erase(iter);
		delete pScr;
	}
}
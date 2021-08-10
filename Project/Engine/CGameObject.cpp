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

#include "CScript.h"


#include "CResourceManager.h"
#include "CPrefab.h"

CGameObject::CGameObject() :
	m_arrComponent{},
	m_pParentObj(nullptr),
	m_eLayer(E_Layer::End),
	m_bDead(false)
{
}

CGameObject::CGameObject(const CGameObject& _origin) :
	m_arrComponent{},
	m_pParentObj(nullptr),
	m_eLayer(E_Layer::End),
	m_bDead(false)
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != _origin.m_arrComponent[i])
			AddComponent((CComponent*)_origin.m_arrComponent[i]->Clone());
	}

	for (UINT i = 0; i < _origin.m_vecScript.size(); ++i)
		AddComponent(_origin.m_vecScript[i]->Clone());

	for (UINT i = 0; i < _origin.m_vecChildObj.size(); ++i)
		_AddChildGameObject(_origin.m_vecChildObj[i]->Clone());
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(m_arrComponent);
	Safe_Delete_Vector(m_vecChildObj);
	Safe_Delete_Vector(m_vecScript);
}

void CGameObject::Awake()
{
	if (IsDead())
		return;
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Awake();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->Awake();

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->Awake();
}

void CGameObject::Start()
{
	if (IsDead())
		return;
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Start();
	}
	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->Start();

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->Start();
}

void CGameObject::PrevUpdate()
{
	if (IsDead())
		return;
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->PrevUpdate();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->PrevUpdate();

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->PrevUpdate();
}

void CGameObject::Update()
{
	if (IsDead())
		return;
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Update();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->Update();

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->Update();
}

void CGameObject::LateUpdate()
{
	if (IsDead())
		return;
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->LateUpdate();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->LateUpdate();

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->LateUpdate();
}

void CGameObject::FinalUpdate()
{
	if (IsDead())
		return;
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->FinalUpdate();
	}
	
	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->FinalUpdate();
	
	_RegisterLayer(); // 레이어 등록
}

void CGameObject::Render()
{
	if (MeshRenderer())		// 메쉬 렌더링
		MeshRenderer()->Render();

	if (TileMap())			// 타일맵 렌더링
		TileMap()->Render();

	if (ParticleSystem())	// 파티클 시스템 렌더링
		ParticleSystem()->Render();

	if (Light2D())			// 광원 렌더링
		Light2D()->Render();

	if (Collider2D())		// 충돌체 렌더링
		Collider2D()->Render();
}

void CGameObject::RegisterAsPrefab(const tstring& _strName)
{
	tstring strName = _strName;
	if (strName.empty()) // 비어있으면 오브젝트 이름을 프리펩 이름으로 설정
		strName = GetName();

	SharedPtr<CPrefab> pPrefab = new CPrefab(this->Clone());
	CResourceManager::GetInstance()->AddRes<CPrefab>(strName, pPrefab.Get());
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

void CGameObject::_AddChildGameObject(CGameObject* _pChildObj)
{
	// 자식 오브젝트가 Ancestor 오브젝트면 안됨
	if (_IsAncestorGameObject(_pChildObj)) {
		assert(nullptr);
		return;
	}
	
	// 자식 오브젝트가 최상위 오브젝트인 경우
	if (nullptr == _pChildObj->GetParentObject() && E_Layer::End != _pChildObj->GetLayer()) {
		// 레이어의 Root들의 오브젝트를 저장하는걸 해제
		CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(_pChildObj->GetLayer());
		pLayer->_UnRegisterInRootGameObject(_pChildObj);
	}

	// 자식 오브젝트가 부모가 있으면 해제
	_pChildObj->_UnlinkParentGameObject();

	// 부모와 자식 연결
	_pChildObj->m_pParentObj = this;
	m_vecChildObj.push_back(_pChildObj);


	// 소속된 레이어가 없으면 부모 오브젝트의 레이어로 설정.
	if (E_Layer::End == _pChildObj->GetLayer())
		_pChildObj->_SetLayer(GetLayer());
}

void CGameObject::_RegisterLayer()
{
	CLayer* pLayer = CSceneManager::GetInstance()->GetCurScene()->GetLayer(m_eLayer);
	pLayer->RegisterGameObject(this);
}

void CGameObject::_UnlinkParentGameObject()
{
	if (nullptr == m_pParentObj)
		return;

	vector<CGameObject*>& vecChild = m_pParentObj->_GetChildsObjectRef();
	auto iter = vecChild.begin();

	for (; iter != vecChild.end(); ++iter) {
		if (*iter == this) {
			vecChild.erase(iter);
			break;
		}
	}

	m_pParentObj = nullptr;
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

	// 자식 오브젝트일 경우
	if (GetParentObject())
		FWrite(m_eLayer, _pFile);
	
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
		
	// 자식 오브젝트
	UINT iChildCount = (UINT)m_vecChildObj.size();
	FWrite(iChildCount, _pFile);

	for (UINT i = 0; i < m_vecChildObj.size(); ++i)
		m_vecChildObj[i]->SaveToScene(_pFile);

	return true;
}

bool CGameObject::LoadFromScene(FILE* _pFile, int _iDepth)
{
	CObject::LoadFromScene(_pFile);

	// 자식 오브젝트인 경우 Layer 소속 읽기
	if (0 != _iDepth)
		FRead(m_eLayer, _pFile);

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
	UINT iScriptCount = (UINT)m_vecScript.size();
	FRead(iScriptCount, _pFile);
	m_vecScript.clear();
	m_vecScript.resize(iScriptCount);
	for (UINT i = 0; i < iScriptCount; ++i) {
		CScript* pScript = CSceneManager::GetInstance()->m_pLoadScript(_pFile);
		AddComponent(pScript);
	}
		
	// 자식 정보
	++_iDepth;

	UINT iChildCount = 0;
	FRead(iChildCount, _pFile);
	for (UINT i = 0; i < iChildCount; ++i) {
		CGameObject* pChildObj = new CGameObject;
		pChildObj->LoadFromScene(_pFile, _iDepth);
		_AddChildGameObject(pChildObj);
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
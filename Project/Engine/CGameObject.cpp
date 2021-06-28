#include "pch.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CMeshRenderer.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CCollider2D.h"
#include "CLight2D.h"
#include "CTileMap.h"

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

	for (UINT i = 0; i < _origin.m_vecChildObj.size(); ++i)
		_AddChildGameObject(_origin.m_vecChildObj[i]->Clone());
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(m_arrComponent);
	if (0 < m_vecChildObj.size()) {
		Safe_Delete_Vector(m_vecChildObj);
	}
}

void CGameObject::Awake()
{
	if (IsDead())
		return;
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Awake();
	}
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

	if (TileMap())
		TileMap()->Render();

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

	vector<CGameObject*> vecChild = m_pParentObj->_GetChildsObjectRef();
	auto iter = vecChild.begin();

	for (; iter != vecChild.end(); ++iter) {
		if (*iter == this) {
			vecChild.erase(iter);
			break;
		}
	}

	m_pParentObj = nullptr;
}

CComponent* CGameObject::AddComponent(CComponent* _pComponent)
{
	if (m_arrComponent[(UINT)_pComponent->GetComponentType()])
		return m_arrComponent[(UINT)_pComponent->GetComponentType()];

	m_arrComponent[(UINT)_pComponent->GetComponentType()] = _pComponent;
	_pComponent->m_pGameObj = this;

	return _pComponent;
}

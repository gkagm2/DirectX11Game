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
#include "CParticleSystem.h"
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
	
	_RegisterLayer(); // ���̾� ���
}

void CGameObject::Render()
{
	if (MeshRenderer())		// �޽� ������
		MeshRenderer()->Render();

	if (TileMap())			// Ÿ�ϸ� ������
		TileMap()->Render();

	if (ParticleSystem())	// ��ƼŬ �ý��� ������
		ParticleSystem()->Render();

	if (Light2D())			// ���� ������
		Light2D()->Render();

	if (Collider2D())		// �浹ü ������
		Collider2D()->Render();
}

void CGameObject::RegisterAsPrefab(const tstring& _strName)
{
	tstring strName = _strName;
	if (strName.empty()) // ��������� ������Ʈ �̸��� ������ �̸����� ����
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
	// �θ�� �ڽ� ����
	_pChildObj->m_pParentObj = this;
	m_vecChildObj.push_back(_pChildObj);

	// �Ҽӵ� ���̾ ������ �θ� ������Ʈ�� ���̾�� ����.
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

	// ���� �ϳ��� �������� �� �ִ� ������ ������Ʈ���� üũ
	bool bIsOnlyOneRenderComponent = _IsOnlyOnePossibleRenderComponent(_pComponent->GetComponentType());
	if (bIsOnlyOneRenderComponent) {
		for (UINT i = ONLY_ONE_POSSIBLE_RENDERING_START_IDX; i < ONLY_ONE_POSSIBLE_RENDERING_END_IDX; ++i) {
			if (_pComponent->GetComponentType() == (E_ComponentType)i)
				continue;
			if (_IsExistComponent((E_ComponentType)i)) {
				assert(nullptr && _T("�̹� �������ϴ� �ٸ� ������Ʈ�� ������"));
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

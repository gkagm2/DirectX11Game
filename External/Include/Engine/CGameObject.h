#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CComponent;
class CTransform;
class CMeshRenderer;
class CCamera;
class CScript;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CLight2D;


class CGameObject : public CObject, ILifeCycleInterface
{
private:
	CComponent* m_arrComponent[(UINT)E_ComponentType::End];
	CGameObject* m_pParentObj;
	vector<CGameObject*> m_vecChildObj;

	E_Layer m_eLayer;
	bool m_bDead;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;

public:
	// Components
	GET_COMPONENT_COMPOTABLE

public:
	void RegisterAsPrefab(const tstring& _strName = _T(""));

public:
	E_Layer GetLayer() { return m_eLayer; }
	bool IsDead() { return m_bDead; }

	CGameObject* GetParentObject() { return m_pParentObj; }
	const vector<CGameObject*>& GetChildsObject() { return m_vecChildObj; }

private:
	void _SetLayer(E_Layer _eLayer) { m_eLayer = _eLayer; }
	void _SetDead();
	void _AddChildGameObject(CGameObject* _pChildObj);
	void _RegisterLayer();

	vector<CGameObject*>& _GetChildsObjectRef() { return m_vecChildObj; }

	void _UnlinkParentGameObject();

public:
	template<typename TYPE>
	TYPE* AddComponent();

	CComponent* AddComponent(CComponent* _pComponent);

	template<typename TYPE>
	TYPE* GetComponent();

public:
	CLONE(CGameObject);
	CGameObject();
	CGameObject(const CGameObject& _origin);
	virtual ~CGameObject() override;

	friend class CEventManager;
	friend class CLayer;
};

template<typename TYPE>
inline TYPE* CGameObject::AddComponent()
{
	TYPE* pComponent = new TYPE();
	CComponent* pComp = dynamic_cast<CComponent*>(pComponent);
	if (nullptr == pComp) {
		delete pComponent;
		assert(nullptr && _T("컴포넌트 타입이 아님."));
		return nullptr;
	}

	E_ComponentType eType = pComp->GetComponentType();

	// 이미 존재하면
	if (m_arrComponent[(UINT)eType]) {
		delete pComponent;
		pComponent = dynamic_cast<TYPE*>(m_arrComponent[(UINT)eType]);
		return pComponent;
	}

	m_arrComponent[(UINT)eType] = pComp;
	pComp->m_pGameObj = this;

 	return pComponent;
}

template<typename TYPE>
inline TYPE* CGameObject::GetComponent()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
     		TYPE* pComponent = dynamic_cast<TYPE*>(m_arrComponent[i]);
		if (nullptr != pComponent) {
			return pComponent;
		}
	}
	return nullptr;
}
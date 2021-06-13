#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CComponent;
class CTransform;
class CMeshRenderer;
class CCamera;

class CGameObject : public CObject, ILifeCycleInterface
{
private:
	CComponent* m_arrComponent[(UINT)E_ComponentType::End];
	CGameObject* m_pParentObj;
	vector<CGameObject*> m_vecChildObj;

	tstring m_strName;

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
	const tstring& GetName() { return m_strName; }
	void SetName(const tstring& _strName) { m_strName = _strName; }

	E_Layer GetLayer() { return m_eLayer; }
	bool IsDead() { return m_bDead; }


	CGameObject* GetParentObject() { return m_pParentObj; }
	vector<CGameObject*>& GetChildsObject() { return m_vecChildObj; }

private:
	void _SetLayer(E_Layer _eLayer) { m_eLayer = _eLayer; }
	void _SetDead();
	void _AddChildGameObject(CGameObject* _pChildObj);
	void _RegisterLayer();

public:
	template<typename TYPE>
	TYPE* AddComponent();
	
	template<typename TYPE>		
	TYPE* GetComponent();

public:
	CGameObject();
	virtual ~CGameObject() override;

	friend class CEventManager;
	friend class CLayer;
};

template<typename TYPE>
inline TYPE* CGameObject::AddComponent()
{
	TYPE* pComponent = nullptr;
	// �̹� �߰��Ǿ������� �� ������Ʈ�� ����
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		pComponent = dynamic_cast<TYPE*>(m_arrComponent[i]);
		if (nullptr != pComponent) {
			// TODO : �̹� ������Ʈ�� �����Ѵٰ� warning ǥ��
			assert(nullptr && _T("������Ʈ�� �̹� �����ϴµ� �߰� �õ���."));
			return pComponent;
		}
	}

	// ������Ʈ ���� �� Ÿ�� Ȯ��
	pComponent = new TYPE();
	CComponent* pComp = dynamic_cast<CComponent*>(pComponent);
	if (nullptr == pComp) {
		delete pComponent;
		assert(nullptr && _T("������Ʈ Ÿ���� �ƴ�."));
		return nullptr;
	}

	E_ComponentType eType = pComp->GetComponentType();
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
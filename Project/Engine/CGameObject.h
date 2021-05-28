#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CComponent;
class CGameObject : public CObject, ILifeCycleInterface
{
private:
	CComponent* m_arrComponent[(UINT)E_ComponentType::end];

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	template<typename TYPE>
	TYPE* AddComponent();
	
	template<typename TYPE>
	TYPE* GetComponent();

public:
	CGameObject();
	virtual ~CGameObject() override;
};

template<typename TYPE>
inline TYPE* CGameObject::AddComponent()
{
	TYPE* pComponent = new TYPE();
	if (nullptr == dynamic_cast<CComponent*>(pComponent)) {
		delete pComponent;
		assert(nullptr || _T("ÄÄÆ÷³ÍÆ® Å¸ÀÔÀÌ ¾Æ´Ô."));
		return nullptr;
	}

	E_ComponentType eType = pComponent->GetComponentType();
	m_arrComponent[(UINT)eType] = pComponent;
	pComponent->m_pGameObj = this;
	return pComponent;
}

template<typename TYPE>
inline TYPE* CGameObject::GetComponent()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::end; ++i) {
		TYPE* pComponent = dynamic_cast<TYPE*>(m_arrComponent[i]);
		if (nullptr != pComponent) {
			return pComponent;
		}
	}
	return nullptr;
}

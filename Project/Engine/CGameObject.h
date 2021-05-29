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
	virtual void Render() override;

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
	TYPE* pComponent = nullptr;
	// 이미 추가되어있으면 그 컴포넌트를 리턴
	for (UINT i = 0; i < (UINT)E_ComponentType::end; ++i) {
		pComponent = dynamic_cast<TYPE*>(m_arrComponent[i]);
		if (nullptr != pComponent) {
			// TODO : 이미 컴포넌트가 존재한다고 warning 표시
			assert(nullptr && _T("컴포넌트가 이미 존재하는데 추가 시도함."));
			return pComponent;
		}
	}

	// 컴포넌트 생성 및 타입 확인
	pComponent = new TYPE();
	CComponent* pComp = dynamic_cast<CComponent*>(pComponent);
	if (nullptr == pComp) {
		delete pComponent;
		assert(nullptr && _T("컴포넌트 타입이 아님."));
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
	for (UINT i = 0; i < (UINT)E_ComponentType::end; ++i) {
		TYPE* pComponent = dynamic_cast<TYPE*>(m_arrComponent[i]);
		if (nullptr != pComponent) {
			return pComponent;
		}
	}
	return nullptr;
}
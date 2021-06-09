#pragma once
#include "Interfaces.h"
#include "CGameObject.h"

class CComponent : public CObject, ILifeCycleInterface
{
private:
	CGameObject* m_pGameObj;
	E_ComponentType m_eType;

public:
	virtual void Awake() override {}
	virtual void Start() override {}
	virtual void PrevUpdate() override {}
	virtual void Update() override {}
	virtual void LateUpdate() override {}
	virtual void FinalUpdate() override {}
	virtual void Render() override {}

public:
	CGameObject* GetGameObject() { return m_pGameObj; }
	E_ComponentType GetComponentType() { return m_eType; }

public:
	// Components
	GET_COMPONENT_FROM_GAMEOBJECT

public:
	CComponent() = delete;
	CComponent(E_ComponentType _eType);
	virtual ~CComponent() override;

	friend class CGameObject;
};
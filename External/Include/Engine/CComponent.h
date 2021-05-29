#pragma once
#include "CObject.h"
#include "Interfaces.h"

class CGameObject;
class CComponent : public CObject, ILifeCycleInterface
{
private:
	CGameObject* m_pGameObj;
	E_ComponentType m_eType;

public:
	E_ComponentType GetComponentType() { return m_eType; }

public:
	virtual void Awake() override {}
	virtual void Start() override {}
	virtual void PrevUpdate() override {}
	virtual void Update() override {}
	virtual void LateUpdate() override {}
	virtual void Render() override {}

public:
	CComponent() = delete;
	CComponent(E_ComponentType _eType);
	virtual ~CComponent() override;

	friend class CGameObject;
};
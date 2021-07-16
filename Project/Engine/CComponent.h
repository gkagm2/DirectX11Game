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
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	virtual CComponent* Clone() = 0;
	CComponent() = delete;
	CComponent(const CComponent& _origin);
	CComponent(E_ComponentType _eType);
	virtual ~CComponent() override;

	friend class CGameObject;
};
#pragma once
#include "CObject.h"

class CComponent;
class CGameObject : public CObject
{
private:
	CComponent* m_arrComponent[(UINT)E_ComponentType::end];

public:
	void Awake();
	void Start();
	void PrevUpdate();
	void Update();
	void LateUpdate();

public:
	CGameObject();
	virtual ~CGameObject() override;
};
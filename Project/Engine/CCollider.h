#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CCollisionManager.h"

class CGameObject;
class CCollider : public CComponent
{
protected:
	int m_iCollisionCount;		  // 이 객체가 다른 충돌체와 충돌한 횟수

	SharedPtr<CMaterial> m_pCollisionMaterial;
	SharedPtr<CMaterial> m_pNonColliedMaterial;

	bool m_bIsTrigger; // 충돌 시 고정 여부 (true : 물체 통과)

protected:
	void IncreaseCollisionCnt() { ++m_iCollisionCount; }
	void DecreaseCollisionCnt() { --m_iCollisionCount; }
	int GetColCount() { return m_iCollisionCount; }

public:
	void SetTrigger(bool _bIsTrigger) { m_bIsTrigger = _bIsTrigger; }
	bool IsTrigger() { return m_bIsTrigger; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CCollider);
	CCollider() = delete;
	CCollider(E_ComponentType _eComponentType);
	CCollider(const CCollider& _origin);
	virtual ~CCollider() override;
};
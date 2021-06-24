#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;
class CCollider : public CComponent
{
protected:
	int m_iCollisionCount;		  // 이 객체가 다른 충돌체와 충돌한 횟수
	
protected:
	void IncreaseCollisionCnt() { ++m_iCollisionCount; }
	void DecreaseCollisionCnt() { --m_iCollisionCount; }
	int GetColCount() { return m_iCollisionCount; }


public:
	CLONE(CCollider);
	CCollider() = delete;
	CCollider(E_ComponentType _eComponentType);
	CCollider(const CCollider& _origin);
	virtual ~CCollider() override;
};
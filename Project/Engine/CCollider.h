#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;
class CCollider : public CComponent
{
protected:
	int m_iCollisionCount;		  // �� ��ü�� �ٸ� �浹ü�� �浹�� Ƚ��
	
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
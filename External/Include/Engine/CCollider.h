#pragma once
#include "CComponent.h"
#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;
class CCollider : public CComponent
{
protected:
	CGameObject* m_pOwnerGameObj; // 이 객체를 가지고 있는 오브젝트
	int m_iCollisionCount;		  // 이 객체가 다른 충돌체와 충돌한 횟수
	
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;

public:
	CCollider() = delete;
	CCollider(E_ComponentType _eComponentType);
	virtual ~CCollider() override;
};